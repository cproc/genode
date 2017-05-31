/*
 * \brief  FFAT file system
 * \author Christian Prochaska
 * \date   2012-07-03
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <file_system/node_handle_registry.h>
#include <file_system_session/rpc_object.h>
#include <root/component.h>
#include <base/attached_rom_dataspace.h>
#include <os/session_policy.h>
#include <util/xml_node.h>
#include <base/heap.h>
#include <base/log.h>

/* local includes */
#include <directory.h>
#include <file.h>
#include <util.h>

/* Genode block backend */
#include <ffat/block.h>

/* ffat includes */
namespace Ffat { extern "C" {
#include <ffat/ff.h>
} }

/*
 * This macro is defined in later versions of the FatFs lib, but not in the
 * one currently used for Genode.
 */
#define f_tell(fp) ((fp)->fptr)


using namespace Genode;


static Lock _ffat_lock;
typedef Lock_guard<Lock> Ffat_lock_guard;


/*************************
 ** File-system service **
 *************************/

namespace File_system {

	class Session_component : public Session_rpc_object
	{
		private:

			Genode::Env          &_env;
			Genode::Allocator    &_heap;

			Directory            &_root;
			Node_handle_registry  _handle_registry;
			bool                  _writable;

			Signal_handler<Session_component> _process_packet_dispatcher;

			/******************************
			 ** Packet-stream processing **
			 ******************************/

			/**
			 * Perform packet operation
			 *
			 * \return true on success, false on failure
			 */
			void _process_packet_op(Packet_descriptor &packet, Node &node)
			{
				void     * const content = tx_sink()->packet_content(packet);
				size_t     const length  = packet.length();
				seek_off_t const offset  = packet.position();

				if (!content || (packet.length() > packet.size())) {
					packet.succeeded(false);
					return;
				}

				/* resulting length */
				size_t res_length = 0;

				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				switch (packet.operation()) {

					case Packet_descriptor::READ:
						res_length = node.read((char *)content, length, offset);
						break;

					case Packet_descriptor::WRITE:
						res_length = node.write((char const *)content, length, offset);
						break;

					case Packet_descriptor::READ_READY:
						/* not supported */
						break;
				}

				packet.length(res_length);
				packet.succeeded(res_length > 0);
			}

			void _process_packet()
			{
				Packet_descriptor packet = tx_sink()->get_packet();

				/* assume failure by default */
				packet.succeeded(false);

				try {
					Node *node = _handle_registry.lookup(packet.handle());
					_process_packet_op(packet, *node);
				}
				catch (Invalid_handle)     { error("Invalid_handle");     }

				/*
				 * The 'acknowledge_packet' function cannot block because we
				 * checked for 'ready_to_ack' in '_process_packets'.
				 */
				tx_sink()->acknowledge_packet(packet);
			}

			/**
			 * Called by signal dispatcher, executed in the context of the main
			 * thread (not serialized with the RPC functions)
			 */
			void _process_packets()
			{
				while (tx_sink()->packet_avail()) {

					/*
					 * Make sure that the '_process_packet' function does not
					 * block.
					 *
					 * If the acknowledgement queue is full, we defer packet
					 * processing until the client processed pending
					 * acknowledgements and thereby emitted a ready-to-ack
					 * signal. Otherwise, the call of 'acknowledge_packet()'
					 * in '_process_packet' would infinitely block the context
					 * of the main thread. The main thread is however needed
					 * for receiving any subsequent 'ready-to-ack' signals.
					 */
					if (!tx_sink()->ready_to_ack())
						return;

					_process_packet();
				}
			}

			/**
			 * Check if string represents a valid path (most start with '/')
			 */
			static void _assert_valid_path(char const *path)
			{
				if (!valid_path(path)) {
					warning("malformed path '", path, "'");
					throw Lookup_failed();
				}
			}

		public:

			/**
			 * Constructor
			 */
			Session_component(Genode::Env       &env,
			                  Genode::Allocator &heap,
			                  size_t             tx_buf_size,
			                  Directory         &root,
			                  bool               writable)
			:
				Session_rpc_object(env.ram().alloc(tx_buf_size),
				                   env.rm(), env.ep().rpc_ep()),
				_env(env), _heap(heap), _root(root), _writable(writable),
				_process_packet_dispatcher(env.ep(), *this,
				                           &Session_component::_process_packets)
			{
				/*
				 * Register '_process_packets' dispatch function as signal
				 * handler for packet-avail and ready-to-ack signals.
				 */
				_tx.sigh_packet_avail(_process_packet_dispatcher);
				_tx.sigh_ready_to_ack(_process_packet_dispatcher);
			}

			/**
			 * Destructor
			 */
			~Session_component()
			{
				Dataspace_capability ds = tx_sink()->dataspace();
				_env.ram().free(static_cap_cast<Ram_dataspace>(ds));
			}

			/***************************
			 ** File_system interface **
			 ***************************/

			File_handle file(Dir_handle dir_handle, Name const &name,
			                 Mode mode, bool create)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				if (!valid_filename(name.string()))
					throw Invalid_name();

				using namespace Ffat;

				FIL ffat_fil;
				BYTE ffat_flags = 0;

				if (!_writable)
					if (create || (mode != STAT_ONLY && mode != READ_ONLY))
						throw Permission_denied();

				if (create)
					ffat_flags |= FA_CREATE_NEW;

				if ((mode == READ_ONLY) || (mode == READ_WRITE))
					ffat_flags |= FA_READ;

				if ((mode == WRITE_ONLY) || (mode == READ_WRITE))
					ffat_flags |= FA_WRITE;

				Absolute_path absolute_path(_root.name());

				try {
					absolute_path.append(_handle_registry.lookup(dir_handle)->name());
					absolute_path.append("/");
					absolute_path.append(name.string());
				} catch (Path_base::Path_too_long) {
					throw Invalid_name();
				}

				FRESULT res = f_open(&ffat_fil, absolute_path.base(), ffat_flags);

				switch(res) {
					case FR_OK: {
						File *file_node = new (&_heap) File(absolute_path.base());
						file_node->ffat_fil(ffat_fil);
						return _handle_registry.alloc(file_node);
					}
					case FR_NO_FILE:
					case FR_NO_PATH:
						throw Lookup_failed();
					case FR_INVALID_NAME:
					case FR_INVALID_DRIVE:
						throw Invalid_name();
					case FR_EXIST:
						throw Node_already_exists();
					case FR_DENIED:
					case FR_WRITE_PROTECTED:
						throw Permission_denied();
					case FR_NOT_READY:
						error("f_open() failed with error code FR_NOT_READY");
						throw Lookup_failed();
					case FR_DISK_ERR:
						error("f_open() failed with error code FR_DISK_ERR");
						throw Lookup_failed();
					case FR_INT_ERR:
						error("f_open() failed with error code FR_INT_ERR");
						throw Lookup_failed();
					case FR_NOT_ENABLED:
						error("f_open() failed with error code FR_NOT_ENABLED");
						throw Lookup_failed();
					case FR_NO_FILESYSTEM:
						error("f_open() failed with error code FR_NO_FILESYSTEM");
						throw Lookup_failed();
					default:
						/* not supposed to occur according to the libffat documentation */
						error("f_open() returned an unexpected error code");
						throw Lookup_failed();
				}
			}

			Symlink_handle symlink(Dir_handle, Name const &name, bool create)
			{
				/* not supported */
				throw Permission_denied();
			}

			Dir_handle dir(Path const &path, bool create)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				if (create && !_writable)
					throw Permission_denied();

				_assert_valid_path(path.string());

				/*
				 *  The 'Directory' constructor removes trailing slashes,
				 *  except for "/"
				 */
				Directory *dir_node = new (&_heap) Directory(path.string());

				using namespace Ffat;

				Absolute_path absolute_path(_root.name());

				try {
					absolute_path.append(dir_node->name());
					absolute_path.remove_trailing('/');
				} catch (Path_base::Path_too_long) {
					throw Name_too_long();
				}

				if (create) {

					if (is_root(dir_node->name()))
						throw Node_already_exists();

					FRESULT res = f_mkdir(absolute_path.base());

					try {
						switch(res) {
							case FR_OK:
								break;
							case FR_NO_PATH:
								throw Lookup_failed();
							case FR_INVALID_NAME:
								throw Name_too_long();
							case FR_INVALID_DRIVE:
								throw Name_too_long();
							case FR_DENIED:
							case FR_WRITE_PROTECTED:
								throw Permission_denied();
							case FR_EXIST:
								throw Node_already_exists();
							case FR_NOT_READY:
								error("f_mkdir() failed with error code FR_NOT_READY");
								throw Lookup_failed();
							case FR_DISK_ERR:
								error("f_mkdir() failed with error code FR_DISK_ERR");
								throw Lookup_failed();
							case FR_INT_ERR:
								error("f_mkdir() failed with error code FR_INT_ERR");
								throw Lookup_failed();
							case FR_NOT_ENABLED:
								error("f_mkdir() failed with error code FR_NOT_ENABLED");
								throw Lookup_failed();
							case FR_NO_FILESYSTEM:
								error("f_mkdir() failed with error code FR_NO_FILESYSTEM");
								throw Lookup_failed();
							default:
								/* not supposed to occur according to the libffat documentation */
								error("f_mkdir() returned an unexpected error code");
								throw Lookup_failed();
						}
					} catch (Exception e) {
						destroy(&_heap, dir_node);
						throw e;
					}
				}

				Ffat::DIR ffat_dir;
				FRESULT f_opendir_res = f_opendir(&ffat_dir, absolute_path.base());

				try {
					switch(f_opendir_res) {
						case FR_OK:
							dir_node->ffat_dir(ffat_dir);
							return _handle_registry.alloc(dir_node);
						case FR_NO_PATH:
							throw Lookup_failed();
						case FR_INVALID_NAME:
							throw Name_too_long();
						case FR_INVALID_DRIVE:
							throw Name_too_long();
						case FR_NOT_READY:
							error("f_opendir() failed with error code FR_NOT_READY");
							throw Lookup_failed();
						case FR_DISK_ERR:
							error("f_opendir() failed with error code FR_DISK_ERR");
							throw Lookup_failed();
						case FR_INT_ERR:
							error("f_opendir() failed with error code FR_INT_ERR");
							throw Lookup_failed();
						case FR_NOT_ENABLED:
							error("f_opendir() failed with error code FR_NOT_ENABLED");
							throw Lookup_failed();
						case FR_NO_FILESYSTEM:
							error("f_opendir() failed with error code FR_NO_FILESYSTEM");
							throw Lookup_failed();
						default:
							/* not supposed to occur according to the libffat documentation */
							error("f_opendir() returned an unexpected error code");
							throw Lookup_failed();
					}
				} catch (Exception e) {
					destroy(&_heap, dir_node);
					throw e;
				}
			}

			Node_handle node(Path const &path)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				if (!valid_path(path.string()))
					throw Lookup_failed();

				Absolute_path absolute_path(_root.name());

				try {
					absolute_path.append(path.string());
					absolute_path.remove_trailing('/');
				} catch (Path_base::Path_too_long) {
					throw Lookup_failed();
				}

				Node *node = new (&_heap) Node(absolute_path.base());

				/* f_stat() does not work for "/" */
				if (!is_root(node->name())) {

					using namespace Ffat;

					FILINFO file_info;
					/* the long file name is not used in this function */
					file_info.lfname = 0;
					file_info.lfsize = 0;

					FRESULT res = f_stat(node->name(), &file_info);

					try {
						switch(res) {
							case FR_OK:
								break;
							case FR_NO_FILE:
							case FR_NO_PATH:
								throw Lookup_failed();
							case FR_INVALID_NAME:
							case FR_INVALID_DRIVE:
								throw Lookup_failed();
							case FR_DISK_ERR:
								error("f_stat() failed with error code FR_DISK_ERR");
								throw Lookup_failed();
							case FR_INT_ERR:
								error("f_stat() failed with error code FR_INT_ERR");
								throw Lookup_failed();
							case FR_NOT_READY:
								error("f_stat() failed with error code FR_NOT_READY");
								throw Lookup_failed();
							case FR_NOT_ENABLED:
								error("f_stat() failed with error code FR_NOT_ENABLED");
								throw Lookup_failed();
							case FR_NO_FILESYSTEM:
								error("f_stat() failed with error code FR_NO_FILESYSTEM");
								throw Lookup_failed();
							default:
								/* not supposed to occur according to the libffat documentation */
								error("f_stat() returned an unexpected error code");
								throw Lookup_failed();
						}
					} catch (Exception e) {
						destroy(&_heap, node);
						throw e;
					}
				}

				return _handle_registry.alloc(node);
			}

			void close(Node_handle handle)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				Node *node;

				try {
					node = _handle_registry.lookup(handle);
				} catch(Invalid_handle) {
					error("close() called with invalid handle");
					return;
				}

				/* free the handle */
				_handle_registry.free(handle);

				File *file = dynamic_cast<File *>(node);
				if (file) {
					using namespace Ffat;

					FRESULT res = f_close(file->ffat_fil());

					/* free the node */
					destroy(&_heap, file);

					switch(res) {
						case FR_OK:
							return;
						case FR_INVALID_OBJECT:
							error("f_close() failed with error code FR_INVALID_OBJECT");
							return;
						case FR_DISK_ERR:
							error("f_close() failed with error code FR_DISK_ERR");
							return;
						case FR_INT_ERR:
							error("f_close() failed with error code FR_INT_ERR");
							return;
						case FR_NOT_READY:
							error("f_close() failed with error code FR_NOT_READY");
							return;
						default:
							/* not supposed to occur according to the libffat documentation */
							error("f_close() returned an unexpected error code");
							return;
					}
				}
			}

			Status status(Node_handle node_handle)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				Status status;
				status.inode = 1;
				status.size  = 0;
				status.mode  = 0;

				Node *node = _handle_registry.lookup(node_handle);

				using namespace Ffat;

				/* f_stat() does not work for the '/' directory */
				if (!is_root(node->name())) {

					FILINFO ffat_file_info;
					/* the long file name is not used in this function */
					ffat_file_info.lfname = 0;
					ffat_file_info.lfsize = 0;

					FRESULT res = f_stat(node->name(), &ffat_file_info);

					switch(res) {
						case FR_OK:
							break;
						case FR_NO_FILE:
							error("f_stat() failed with error code FR_NO_FILE");
							return status;
						case FR_NO_PATH:
							error("f_stat() failed with error code FR_NO_PATH");
							return status;
						case FR_INVALID_NAME:
							error("f_stat() failed with error code FR_INVALID_NAME");
							return status;
						case FR_INVALID_DRIVE:
							error("f_stat() failed with error code FR_INVALID_DRIVE");
							return status;
						case FR_DISK_ERR:
							error("f_stat() failed with error code FR_DISK_ERR");
							return status;
						case FR_INT_ERR:
							error("f_stat() failed with error code FR_INT_ERR");
							return status;
						case FR_NOT_READY:
							error("f_stat() failed with error code FR_NOT_READY");
							return status;
						case FR_NOT_ENABLED:
							error("f_stat() failed with error code FR_NOT_ENABLED");
							return status;
						case FR_NO_FILESYSTEM:
							error("f_stat() failed with error code FR_NO_FILESYSTEM");
							return status;
						default:
							/* not supposed to occur according to the libffat documentation */
							error("f_stat() returned an unexpected error code");
							return status;
					}

					if ((ffat_file_info.fattrib & AM_DIR) == AM_DIR) {
						status.mode = File_system::Status::MODE_DIRECTORY; }
					else {
						status.mode = File_system::Status::MODE_FILE;
						status.size = ffat_file_info.fsize;
					}

				} else {
					status.mode = File_system::Status::MODE_DIRECTORY;
				}

				if (status.mode == File_system::Status::MODE_DIRECTORY) {

					/* determine the number of directory entries */

					Ffat::DIR ffat_dir;
					FRESULT f_opendir_res = f_opendir(&ffat_dir, node->name());

					if (f_opendir_res != FR_OK)
						return status;

					FILINFO ffat_file_info;
					/* the long file name is not used in this function */
					ffat_file_info.lfname = 0;
					ffat_file_info.lfsize = 0;

					int num_direntries = -1;
					do {
						++num_direntries;
						FRESULT res = f_readdir(&ffat_dir, &ffat_file_info);
						if (res != FR_OK)
							return status;
					} while (ffat_file_info.fname[0] != 0);

					status.size = num_direntries * sizeof(Directory_entry);
				}

				return status;
			}

			void control(Node_handle, Control) { }

			void unlink(Dir_handle dir_handle, Name const &name)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				if (!valid_filename(name.string()))
					throw Invalid_name();

				if (!_writable)
					throw Permission_denied();

				using namespace Ffat;

				Absolute_path absolute_path(_root.name());

				try {
					absolute_path.append(_handle_registry.lookup(dir_handle)->name());
					absolute_path.append("/");
					absolute_path.append(name.string());
				} catch (Path_base::Path_too_long) {
					throw Invalid_name();
				}

				FRESULT res = f_unlink(absolute_path.base());

				switch(res) {
					case FR_OK:
						break;
					case FR_NO_FILE:
					case FR_NO_PATH:
						throw Lookup_failed();
					case FR_INVALID_NAME:
					case FR_INVALID_DRIVE:
						throw Invalid_name();
					case FR_DENIED:
					case FR_WRITE_PROTECTED:
						throw Permission_denied();
					case FR_DISK_ERR:
						error("f_unlink() failed with error code FR_DISK_ERR");
						return;
					case FR_INT_ERR:
						error("f_unlink() failed with error code FR_INT_ERR");
						return;
					case FR_NOT_READY:
						error("f_unlink() failed with error code FR_NOT_READY");
						return;
					case FR_NOT_ENABLED:
						error("f_unlink() failed with error code FR_NOT_ENABLED");
						return;
					case FR_NO_FILESYSTEM:
						error("f_unlink() failed with error code FR_NO_FILESYSTEM");
						return;
					default:
						/* not supposed to occur according to the libffat documentation */
						error("f_unlink() returned an unexpected error code");
						return;
				}
			}

			void truncate(File_handle file_handle, file_size_t size)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				if (!_writable)
					throw Permission_denied();

				File *file = _handle_registry.lookup(file_handle);

				using namespace Ffat;

				/* 'f_truncate()' truncates to the current seek pointer */

				FRESULT res = f_lseek(file->ffat_fil(), size);

				switch(res) {
					case FR_OK:
						/* according to the FatFs documentation this can happen */
						if (f_tell(file->ffat_fil()) != size) {
							error("f_lseek() could not seek to offset ", size);
							return;
						}
						break;
					case FR_DISK_ERR:
						error("f_lseek() failed with error code FR_DISK_ERR");
						return;
					case FR_INT_ERR:
						error("f_lseek() failed with error code FR_INT_ERR");
						return;
					case FR_NOT_READY:
						error("f_lseek() failed with error code FR_NOT_READY");
						return;
					case FR_INVALID_OBJECT:
						error("f_lseek() failed with error code FR_INVALID_OBJECT");
						throw Invalid_handle();
					default:
						/* not supposed to occur according to the libffat documentation */
						error("f_lseek() returned an unexpected error code");
						return;
				}

				res = f_truncate(file->ffat_fil());

				switch(res) {
					case FR_OK:
						return;
					case FR_INVALID_OBJECT:
						error("f_truncate() failed with error code FR_INVALID_OBJECT");
						throw Invalid_handle();
					case FR_DISK_ERR:
						error("f_truncate() failed with error code FR_DISK_ERR");
						return;
					case FR_INT_ERR:
						error("f_truncate() failed with error code FR_INT_ERR");
						return;
					case FR_NOT_READY:
						error("f_truncate() failed with error code FR_NOT_READY");
						return;
					case FR_TIMEOUT:
						error("f_truncate() failed with error code FR_TIMEOUT");
						return;
					default:
						/* not supposed to occur according to the libffat documentation */
						error("f_truncate() returned an unexpected error code");
						return;
				}
			}

			void move(Dir_handle from_dir_handle, Name const &from_name,
			          Dir_handle to_dir_handle,   Name const &to_name)
			{
				Ffat_lock_guard ffat_lock_guard(_ffat_lock);

				if (!_writable)
					throw Permission_denied();

				if (!valid_filename(from_name.string()))
					throw Lookup_failed();

				if (!valid_filename(to_name.string()))
					throw Invalid_name();

				Absolute_path absolute_from_path(_root.name());
				Absolute_path absolute_to_path(_root.name());

				try {
					absolute_from_path.append(_handle_registry.lookup(from_dir_handle)->name());
					absolute_from_path.append("/");
					absolute_from_path.append(from_name.string());
					absolute_to_path.append(_handle_registry.lookup(to_dir_handle)->name());
					absolute_to_path.append("/");
					absolute_to_path.append(to_name.string());
				} catch (Path_base::Path_too_long) {
					throw Invalid_name();
				}

				using namespace Ffat;

				FRESULT res = f_rename(absolute_from_path.base(), absolute_to_path.base());

				switch(res) {
					case FR_OK:
						break;
					case FR_NO_FILE:
					case FR_NO_PATH:
						throw Lookup_failed();
					case FR_INVALID_NAME:
					case FR_INVALID_DRIVE:
						throw Invalid_name();
					case FR_EXIST:
						error("f_rename() failed with error code FR_EXIST");
						throw Invalid_name();
					case FR_DENIED:
					case FR_WRITE_PROTECTED:
						throw Permission_denied();
					case FR_DISK_ERR:
						error("f_rename() failed with error code FR_DISK_ERR");
						throw Lookup_failed();
					case FR_INT_ERR:
						error("f_rename() failed with error code FR_INT_ERR");
						throw Lookup_failed();
					case FR_NOT_READY:
						error("f_rename() failed with error code FR_NOT_READY");
						throw Lookup_failed();
					case FR_NOT_ENABLED:
						error("f_rename() failed with error code FR_NOT_ENABLED");
						throw Lookup_failed();
					case FR_NO_FILESYSTEM:
						error("f_rename() failed with error code FR_NO_FILESYSTEM");
						throw Lookup_failed();
					default:
						/* not supposed to occur according to the libffat documentation */
						error("f_rename() returned an unexpected error code");
						throw Lookup_failed();
				}
			}

			void sigh(Node_handle, Genode::Signal_context_capability)
			{
				error("File_system::Session::sigh not supported");
			}
	};


	class Root : public Root_component<Session_component>
	{
		private:

			Genode::Env       &_env;
			Genode::Allocator &_md_alloc;
			Genode::Allocator &_heap;

			Directory         &_root_dir;

		protected:

			Session_component *_create_session(const char *args)
			{
				/*
				 * Determine client-specific policy defined implicitly by
				 * the client's label.
				 */
				Directory *session_root_dir = 0;
				bool writeable = false;

				enum { ROOT_MAX_LEN = 256 };
				char root[ROOT_MAX_LEN];
				root[0] = 0;

				Session_label const label = label_from_args(args);
				try {
					Session_policy policy(label);

					/*
					 * Determine directory that is used as root directory of
					 * the session.
					 */
					try {
						policy.attribute("root").value(root, sizeof(root));
						if (is_root(root)) {
							session_root_dir = &_root_dir;
						} else {
							/*
							 * Make sure the root path is specified with a
							 * leading path delimiter. For performing the
							 * lookup, we skip the first character.
							 */
							if (root[0] != '/')
								throw Lookup_failed();

							/* Check if the root path exists */

							using namespace Ffat;

							FRESULT res = f_chdir(root);

							switch(res) {
								case FR_OK:
									break;
								case FR_NO_PATH:
									throw Lookup_failed();
								case FR_INVALID_NAME:
								case FR_INVALID_DRIVE:
									throw Lookup_failed();
								case FR_NOT_READY:
									error("f_chdir() failed with error code FR_NOT_READY");
									throw Service_denied();
								case FR_DISK_ERR:
									error("f_chdir() failed with error code FR_DISK_ERR");
									throw Service_denied();
								case FR_INT_ERR:
									error("f_chdir() failed with error code FR_INT_ERR");
									throw Service_denied();
								case FR_NOT_ENABLED:
									error("f_chdir() failed with error code FR_NOT_ENABLED");
									throw Service_denied();
								case FR_NO_FILESYSTEM:
									error("f_chdir() failed with error code FR_NO_FILESYSTEM");
									throw Service_denied();
								default:
									/* not supposed to occur according to the libffat documentation */
									error("f_chdir() returned an unexpected error code");
									throw Service_denied();
							}

							session_root_dir = new (&_md_alloc) Directory(root);
						}
					}
					catch (Xml_node::Nonexistent_attribute) {
						error("missing \"root\" attribute in policy definition");
						throw Service_denied();
					}
					catch (Lookup_failed) {
						error("session root directory \"", Cstring(root), "\" does not exist");
						throw Service_denied();
					}

					/*
					 * Determine if write access is permitted for the session.
					 */
					writeable = policy.attribute_value("writeable", false);
				}
				catch (Session_policy::No_policy_defined) {
					error("Invalid session request, no matching policy");
					throw Service_denied();
				}

				size_t ram_quota =
					Arg_string::find_arg(args, "ram_quota"  ).ulong_value(0);
				size_t tx_buf_size =
					Arg_string::find_arg(args, "tx_buf_size").ulong_value(0);

				if (!tx_buf_size) {
					error(label, " requested a session with a zero length transmission buffer");
					throw Service_denied();
				}

				/*
				 * Check if donated ram quota suffices for session data,
				 * and communication buffer.
				 */
				size_t session_size = sizeof(Session_component) + tx_buf_size;
				if (max((size_t)4096, session_size) > ram_quota) {
					error("insufficient 'ram_quota', got ", ram_quota, ", "
					      "need ", session_size);
					throw Insufficient_ram_quota();
				}
				return new (md_alloc())
					Session_component(_env, _heap, tx_buf_size,
					                  *session_root_dir, writeable);
			}

		public:

			/**
			 * Constructor
			 *
			 * \param env   reference to Genode environment
			 * \param heap  meta-data allocator
			 * \param root  normal root directory if root in policy starts
			 *              at root
			 */
			Root(Genode::Env &env, Allocator &md_alloc, Genode::Allocator &heap,
			     Directory &root)
			:
				Root_component<Session_component>(&env.ep().rpc_ep(), &md_alloc),
				_env(env), _md_alloc(md_alloc), _heap(heap), _root_dir(root)
			{ }
	};
};


struct Main
{
	Genode::Env         &_env;
	Genode::Heap         _heap        { _env.ram(), _env.rm() };
	Genode::Sliced_heap  _sliced_heap { _env.ram(), _env.rm() };

	File_system::Directory _root_dir { "/" };
	File_system::Root      _root { _env, _sliced_heap, _heap, _root_dir };

	Ffat::FATFS _fatfs;

	Main(Genode::Env &env) : _env(env)
	{
		Ffat::block_init(_env, _heap);

		using namespace File_system;
		using namespace Ffat;

		/* mount the file system */
		if (f_mount(0, &_fatfs) != Ffat::FR_OK) {
			error("mount failed");

			struct Mount_failed : Genode::Exception { };
			throw Mount_failed();
		}

		Genode::log("--- Starting Ffat_fs ---");

		_env.parent().announce(_env.ep().manage(_root));
	}
};


void Component::construct(Genode::Env &env) { static Main main(env); }
