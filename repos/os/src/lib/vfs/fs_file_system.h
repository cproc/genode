/*
 * \brief  Adapter from Genode 'File_system' session to VFS
 * \author Norman Feske
 * \author Emery Hemingway
 * \author Christian Helmuth
 * \date   2011-02-17
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__VFS__FS_FILE_SYSTEM_H_
#define _INCLUDE__VFS__FS_FILE_SYSTEM_H_

/* Genode includes */
#include <base/allocator_avl.h>
#include <base/id_space.h>
#include <file_system_session/connection.h>

namespace Vfs { class Fs_file_system; }


class Vfs::Fs_file_system : public File_system
{
	private:

		/*
		 * Lock used to serialize the interaction with the packet stream of the
		 * file-system session.
		 *
		 * XXX Once, we change the VFS file-system interface to use
		 *     asynchronous read/write operations, we can possibly remove it.
		 */
		Lock _lock;

		Genode::Env           &_env;
		Genode::Allocator_avl  _fs_packet_alloc;

		typedef Genode::String<64> Label_string;
		Label_string _label;

		typedef Genode::String<::File_system::MAX_NAME_LEN> Root_string;
		Root_string _root;

		::File_system::Connection _fs;

		class Fs_vfs_handle;

		struct Handle_space : Genode::Id_space<Fs_vfs_handle>
		{
			struct Id : Genode::Id_space<Fs_vfs_handle>::Id
			{
				Id(unsigned long v)              { value = v; }
				Id(::File_system::Node_handle h) { value = h.value; }
			};
		};

		struct Fs_vfs_handle : public Vfs_handle, public Handle_space::Element
		{
			Handle_space::Id const id;

			enum State { IDLE, PENDING, READY };
			State read_state = IDLE;

			enum class Queued_read { IDLE, QUEUED, ACK };
			Queued_read queued_read_state = Queued_read::IDLE;

			::File_system::Packet_descriptor queued_read_packet;

			Fs_vfs_handle(File_system &fs, Allocator &alloc, int status_flags,
			              Handle_space &space, Handle_space::Id id)
			:
				Vfs_handle(fs, fs, alloc, status_flags),
				Handle_space::Element(*this, space, id),
				id(id)
			{ }

			::File_system::File_handle file_handle() const { return id.value; }
		};

		/**
		 * Helper for managing the lifetime of temporary open node handles
		 */
		struct Fs_handle_guard
		{
			::File_system::Session     &_fs;
			::File_system::Node_handle  _handle;

			Fs_handle_guard(::File_system::Session &fs,
			                ::File_system::Node_handle handle)
			: _fs(fs), _handle(handle) { }

			~Fs_handle_guard() { _fs.close(_handle); }
		};

		Handle_space _handle_space;

		file_size _read(::File_system::Node_handle node_handle, void *buf,
		                file_size const count, file_size const seek_offset)
		{
			::File_system::Session::Tx::Source &source = *_fs.tx();

			file_size const max_packet_size = source.bulk_buffer_size() / 2;
			file_size const clipped_count = min(max_packet_size, count);

			::File_system::Packet_descriptor const
				packet_in(source.alloc_packet(clipped_count),
				          node_handle,
				          ::File_system::Packet_descriptor::READ,
				          clipped_count,
				          seek_offset);

			/* pass packet to server side */
			source.submit_packet(packet_in);

while (!source.ack_avail()) /* FIXME busy wait */;
			/* obtain result packet descriptor with updated status info */
			::File_system::Packet_descriptor const
				packet_out = source.get_acked_packet();

			file_size const read_num_bytes = min(packet_out.length(), count);

			memcpy(buf, source.packet_content(packet_out), read_num_bytes);

			/*
			 * XXX check if acked packet belongs to request,
			 *     needed for thread safety
			 */

			source.release_packet(packet_out);

			return read_num_bytes;
		}

		file_size _write(::File_system::Node_handle node_handle,
		                 const char *buf, file_size count, file_size seek_offset)
		{
			::File_system::Session::Tx::Source &source = *_fs.tx();

			file_size const max_packet_size = source.bulk_buffer_size() / 2;
			count = min(max_packet_size, count);

			::File_system::Packet_descriptor
				packet(source.alloc_packet(count),
				       node_handle,
				       ::File_system::Packet_descriptor::WRITE,
				       count,
				       seek_offset);

			memcpy(source.packet_content(packet), buf, count);

			/* pass packet to server side */
			source.submit_packet(packet);

while (!source.ack_avail()) /* FIXME busy wait */;
			/* obtain result packet descriptor with updated status info */
			::File_system::Packet_descriptor const
				packet_out = source.get_acked_packet();

			/*
			 * XXX check if acked packet belongs to request,
			 *     needed for thread safety
			 */

			file_size const write_num_bytes = min(packet_out.length(), count);

			source.release_packet(packet);

			return write_num_bytes;
		}

		Genode::Signal_context_capability _read_ready_sigh;

		void _handle_ack()
		{
			::File_system::Session::Tx::Source &source = *_fs.tx();
			using ::File_system::Packet_descriptor;

			while (source.ack_avail()) {

				Packet_descriptor const packet = source.get_acked_packet();

				Handle_space::Id const id(packet.handle());

				_handle_space.apply<Fs_vfs_handle>(id, [&] (Fs_vfs_handle &handle)
				{
					switch (packet.operation()) {
					case Packet_descriptor::READ_READY:
						handle.read_state = Fs_vfs_handle::READY;
						break;

					case Packet_descriptor::READ:
						handle.queued_read_packet = packet;
						handle.queued_read_state = Fs_vfs_handle::Queued_read::ACK;
						break;

					case Packet_descriptor::WRITE:
						source.release_packet(packet);
						break;

					case Packet_descriptor::INVALID:
						Genode::error("received acknowledgment for an unhandled packet");
						return;
					}

					if (_read_ready_sigh.valid())
						Genode::Signal_transmitter(_read_ready_sigh).submit();
				});
			}
		}

		Genode::Signal_handler<Fs_file_system> _ack_handler {
			_env.ep(), *this, &Fs_file_system::_handle_ack };

	public:

		Fs_file_system(Genode::Env       &env,
		               Genode::Allocator &alloc,
		               Genode::Xml_node   config)
		:
			_env(env),
			_fs_packet_alloc(&alloc),
			_label(config.attribute_value("label", Label_string())),
			_root( config.attribute_value("root",  Root_string())),
			_fs(_fs_packet_alloc,
			    ::File_system::DEFAULT_TX_BUF_SIZE,
			    _label.string(), _root.string(),
			    config.attribute_value("writeable", true))
		{
			_fs.sigh_ack_avail(_ack_handler);
		}


		/*********************************
		 ** Directory-service interface **
		 *********************************/

		Dataspace_capability dataspace(char const *path) override
		{
			Lock::Guard guard(_lock);

			Absolute_path dir_path(path);
			dir_path.strip_last_element();

			Absolute_path file_name(path);
			file_name.keep_only_last_element();

			Ram_dataspace_capability ds_cap;
			char *local_addr = 0;

			try {
				::File_system::Dir_handle dir = _fs.dir(dir_path.base(),
				                                        false);
				Fs_handle_guard dir_guard(_fs, dir);

				::File_system::File_handle file =
				    _fs.file(dir, file_name.base() + 1,
				             ::File_system::READ_ONLY, false);
				Fs_handle_guard file_guard(_fs, file);

				::File_system::Status status = _fs.status(file);

				Ram_dataspace_capability ds_cap =
				    _env.ram().alloc(status.size);

				local_addr = _env.rm().attach(ds_cap);

				::File_system::Session::Tx::Source &source = *_fs.tx();
				file_size const max_packet_size = source.bulk_buffer_size() / 2;

				for (file_size seek_offset = 0; seek_offset < status.size;
				     seek_offset += max_packet_size) {

					file_size const count = min(max_packet_size, status.size -
					                                             seek_offset);

					::File_system::Packet_descriptor
						packet(source.alloc_packet(count),
							   file,
							   ::File_system::Packet_descriptor::READ,
							   count,
							   seek_offset);

					/* pass packet to server side */
					source.submit_packet(packet);
					source.get_acked_packet();

					memcpy(local_addr + seek_offset, source.packet_content(packet), count);

					/*
					 * XXX check if acked packet belongs to request,
					 *     needed for thread safety
					 */

					source.release_packet(packet);
				}

				_env.rm().detach(local_addr);

				return ds_cap;
			} catch(...) {
				_env.rm().detach(local_addr);
				_env.ram().free(ds_cap);
				return Dataspace_capability();
			}
		}

		void release(char const *path, Dataspace_capability ds_cap) override
		{
			if (ds_cap.valid())
				_env.ram().free(static_cap_cast<Genode::Ram_dataspace>(ds_cap));
		}

		Stat_result stat(char const *path, Stat &out) override
		{
			::File_system::Status status;

			try {
				::File_system::Node_handle node = _fs.node(path);
				Fs_handle_guard node_guard(_fs, node);
				status = _fs.status(node);
			}
			catch (::File_system::Lookup_failed)   { return STAT_ERR_NO_ENTRY; }
			catch (::File_system::Out_of_metadata) { return STAT_ERR_NO_PERM;  }

			out = Stat();

			out.size = status.size;
			out.mode = STAT_MODE_FILE | 0777;

			if (status.symlink())
				out.mode = STAT_MODE_SYMLINK | 0777;

			if (status.directory())
				out.mode = STAT_MODE_DIRECTORY | 0777;

			out.uid = 0;
			out.gid = 0;
			out.inode = status.inode;
			out.device = (Genode::addr_t)this;
			return STAT_OK;
		}

		Dirent_result dirent(char const *path, file_offset index, Dirent &out) override
		{
			Lock::Guard guard(_lock);

			::File_system::Session::Tx::Source &source = *_fs.tx();

			if (strcmp(path, "") == 0)
				path = "/";

			::File_system::Dir_handle dir_handle;
			try { dir_handle = _fs.dir(path, false); }
			catch (::File_system::Lookup_failed) { return DIRENT_ERR_INVALID_PATH; }
			catch (::File_system::Name_too_long) { return DIRENT_ERR_INVALID_PATH; }
			catch (...) { return DIRENT_ERR_NO_PERM; }
			Fs_handle_guard dir_guard(_fs, dir_handle);

			enum { DIRENT_SIZE = sizeof(::File_system::Directory_entry) };

			::File_system::Packet_descriptor
				packet(source.alloc_packet(DIRENT_SIZE),
				       dir_handle,
				       ::File_system::Packet_descriptor::READ,
				       DIRENT_SIZE,
				       index*DIRENT_SIZE);

			/* pass packet to server side */
			source.submit_packet(packet);
			source.get_acked_packet();

			/*
			 * XXX check if acked packet belongs to request,
			 *     needed for thread safety
			 */

			typedef ::File_system::Directory_entry Directory_entry;

			/* copy-out payload into destination buffer */
			Directory_entry const *entry =
				(Directory_entry *)source.packet_content(packet);

			/*
			 * The default value has no meaning because the switch below
			 * assigns a value in each possible branch. But it is needed to
			 * keep the compiler happy.
			 */
			Dirent_type type = DIRENT_TYPE_END;

			switch (entry->type) {
			case Directory_entry::TYPE_DIRECTORY: type = DIRENT_TYPE_DIRECTORY; break;
			case Directory_entry::TYPE_FILE:      type = DIRENT_TYPE_FILE;      break;
			case Directory_entry::TYPE_SYMLINK:   type = DIRENT_TYPE_SYMLINK;   break;
			}

			out.fileno = entry->inode;
			out.type   = type;
			strncpy(out.name, entry->name, sizeof(out.name));

			source.release_packet(packet);

			return DIRENT_OK;
		}

		Unlink_result unlink(char const *path) override
		{
			Absolute_path dir_path(path);
			dir_path.strip_last_element();

			Absolute_path file_name(path);
			file_name.keep_only_last_element();

			try {
				::File_system::Dir_handle dir = _fs.dir(dir_path.base(), false);
				Fs_handle_guard dir_guard(_fs, dir);

				_fs.unlink(dir, file_name.base() + 1);
			}
			catch (::File_system::Invalid_handle)    { return UNLINK_ERR_NO_ENTRY;  }
			catch (::File_system::Invalid_name)      { return UNLINK_ERR_NO_ENTRY;  }
			catch (::File_system::Lookup_failed)     { return UNLINK_ERR_NO_ENTRY;  }
			catch (::File_system::Not_empty)         { return UNLINK_ERR_NOT_EMPTY; }
			catch (::File_system::Permission_denied) { return UNLINK_ERR_NO_PERM;   }

			return UNLINK_OK;
		}

		Readlink_result readlink(char const *path, char *buf, file_size buf_size,
		                         file_size &out_len) override
		{
			/*
			 * Canonicalize path (i.e., path must start with '/')
			 */
			Absolute_path abs_path(path);
			abs_path.strip_last_element();

			Absolute_path symlink_name(path);
			symlink_name.keep_only_last_element();

			try {
				::File_system::Dir_handle dir_handle = _fs.dir(abs_path.base(), false);
				Fs_handle_guard from_dir_guard(_fs, dir_handle);

				::File_system::Symlink_handle symlink_handle =
				    _fs.symlink(dir_handle, symlink_name.base() + 1, false);
				Fs_handle_guard symlink_guard(_fs, symlink_handle);

				out_len = _read(symlink_handle, buf, buf_size, 0);

				return READLINK_OK;
			}
			catch (::File_system::Lookup_failed)  { return READLINK_ERR_NO_ENTRY; }
			catch (::File_system::Invalid_handle) { return READLINK_ERR_NO_ENTRY; }
			catch (...) { return READLINK_ERR_NO_PERM; }
		}

		Rename_result rename(char const *from_path, char const *to_path) override
		{
			if ((strcmp(from_path, to_path) == 0) && leaf_path(from_path))
				return RENAME_OK;

			Absolute_path from_dir_path(from_path);
			from_dir_path.strip_last_element();

			Absolute_path from_file_name(from_path);
			from_file_name.keep_only_last_element();

			Absolute_path to_dir_path(to_path);
			to_dir_path.strip_last_element();

			Absolute_path to_file_name(to_path);
			to_file_name.keep_only_last_element();

			try {
				::File_system::Dir_handle from_dir = _fs.dir(from_dir_path.base(), false);
				Fs_handle_guard from_dir_guard(_fs, from_dir);
				::File_system::Dir_handle to_dir = _fs.dir(to_dir_path.base(), false);
				Fs_handle_guard to_dir_guard(_fs, to_dir);

				_fs.move(from_dir, from_file_name.base() + 1,
				         to_dir,   to_file_name.base() + 1);
			}
			catch (::File_system::Lookup_failed) { return RENAME_ERR_NO_ENTRY; }
			catch (...)                          { return RENAME_ERR_NO_PERM; }

			return RENAME_OK;
		}

		Mkdir_result mkdir(char const *path, unsigned mode) override
		{
			/*
			 * Canonicalize path (i.e., path must start with '/')
			 */
			Absolute_path abs_path(path);

			try {
				_fs.close(_fs.dir(abs_path.base(), true));
			}
			catch (::File_system::Permission_denied)   { return MKDIR_ERR_NO_PERM; }
			catch (::File_system::Node_already_exists) { return MKDIR_ERR_EXISTS; }
			catch (::File_system::Lookup_failed)       { return MKDIR_ERR_NO_ENTRY; }
			catch (::File_system::Name_too_long)       { return MKDIR_ERR_NAME_TOO_LONG; }
			catch (::File_system::No_space)            { return MKDIR_ERR_NO_SPACE; }
			catch (::File_system::Out_of_metadata)     { return MKDIR_ERR_NO_ENTRY; }

			return MKDIR_OK;
		}

		Symlink_result symlink(char const *from, char const *to) override
		{
			/*
			 * We write to the symlink via the packet stream. Hence we need
			 * to serialize with other packet-stream operations.
			 */
			Lock::Guard guard(_lock);

			/*
			 * Canonicalize path (i.e., path must start with '/')
			 */
			Absolute_path abs_path(to);
			abs_path.strip_last_element();

			Absolute_path symlink_name(to);
			symlink_name.keep_only_last_element();

			try {
				::File_system::Dir_handle dir_handle = _fs.dir(abs_path.base(), false);
				Fs_handle_guard from_dir_guard(_fs, dir_handle);

				::File_system::Symlink_handle symlink_handle =
				    _fs.symlink(dir_handle, symlink_name.base() + 1, true);
				Fs_handle_guard symlink_guard(_fs, symlink_handle);

				_write(symlink_handle, from, strlen(from) + 1, 0);
			}
			catch (::File_system::Invalid_handle)      { return SYMLINK_ERR_NO_ENTRY; }
			catch (::File_system::Node_already_exists) { return SYMLINK_ERR_EXISTS;   }
			catch (::File_system::Invalid_name)        { return SYMLINK_ERR_NAME_TOO_LONG; }
			catch (::File_system::Lookup_failed)       { return SYMLINK_ERR_NO_ENTRY; }
			catch (::File_system::Permission_denied)   { return SYMLINK_ERR_NO_PERM;  }
			catch (::File_system::No_space)            { return SYMLINK_ERR_NO_SPACE; }
			catch (::File_system::Out_of_metadata)     { return SYMLINK_ERR_NO_ENTRY; }

			return SYMLINK_OK;
		}

		file_size num_dirent(char const *path) override
		{
			if (strcmp(path, "") == 0)
				path = "/";

			::File_system::Node_handle node;
			try { node = _fs.node(path); } catch (...) { return 0; }
			Fs_handle_guard node_guard(_fs, node);

			::File_system::Status status = _fs.status(node);

			return status.size / sizeof(::File_system::Directory_entry);
		}

		bool directory(char const *path) override
		{
			try {
				::File_system::Node_handle node = _fs.node(path);
				Fs_handle_guard node_guard(_fs, node);

				::File_system::Status status = _fs.status(node);

				return status.directory();
			}
			catch (...) { return false; }
		}

		char const *leaf_path(char const *path) override
		{
			/* check if node at path exists within file system */
			try {
				::File_system::Node_handle node = _fs.node(path);
				_fs.close(node);
			}
			catch (...) { return 0; }

			return path;
		}

		Open_result open(char const *path, unsigned vfs_mode, Vfs_handle **out_handle, Genode::Allocator& alloc) override
		{
			Lock::Guard guard(_lock);

			Absolute_path dir_path(path);
			dir_path.strip_last_element();

			Absolute_path file_name(path);
			file_name.keep_only_last_element();

			::File_system::Mode mode;

			switch (vfs_mode & OPEN_MODE_ACCMODE) {
			default:               mode = ::File_system::STAT_ONLY;  break;
			case OPEN_MODE_RDONLY: mode = ::File_system::READ_ONLY;  break;
			case OPEN_MODE_WRONLY: mode = ::File_system::WRITE_ONLY; break;
			case OPEN_MODE_RDWR:   mode = ::File_system::READ_WRITE; break;
			}

			bool const create = vfs_mode & OPEN_MODE_CREATE;

			try {
				::File_system::Dir_handle dir = _fs.dir(dir_path.base(), false);
				Fs_handle_guard dir_guard(_fs, dir);

				::File_system::File_handle file = _fs.file(dir, file_name.base() + 1,
				                                           mode, create);

				Handle_space::Id id { file };
				*out_handle = new (alloc) Fs_vfs_handle(*this, alloc, vfs_mode, _handle_space, id);
			}
			catch (::File_system::Lookup_failed)       { return OPEN_ERR_UNACCESSIBLE;  }
			catch (::File_system::Permission_denied)   { return OPEN_ERR_NO_PERM;       }
			catch (::File_system::Invalid_handle)      { return OPEN_ERR_UNACCESSIBLE;  }
			catch (::File_system::Node_already_exists) { return OPEN_ERR_EXISTS;        }
			catch (::File_system::Invalid_name)        { return OPEN_ERR_NAME_TOO_LONG; }
			catch (::File_system::Name_too_long)       { return OPEN_ERR_NAME_TOO_LONG; }
			catch (::File_system::No_space)            { return OPEN_ERR_NO_SPACE;      }
			catch (::File_system::Out_of_metadata)     { return OPEN_ERR_NO_PERM;       }

			return OPEN_OK;
		}

		void close(Vfs_handle *vfs_handle) override
		{
			if (!vfs_handle) return;

			Lock::Guard guard(_lock);

			Fs_vfs_handle *fs_handle = static_cast<Fs_vfs_handle *>(vfs_handle);

			if (fs_handle) {
				_fs.close(fs_handle->file_handle());
				destroy(fs_handle->alloc(), fs_handle);
			}
		}


		/***************************
		 ** File_system interface **
		 ***************************/

		static char const *name() { return "fs"; }

		void sync(char const *path) override
		{
			try {
				::File_system::Node_handle node = _fs.node(path);
				_fs.sync(node);
				_fs.close(node);
			} catch (...) { }
		}


		/********************************
		 ** File I/O service interface **
		 ********************************/

		Write_result write(Vfs_handle *vfs_handle, char const *buf,
		                   file_size buf_size, file_size &out_count) override
		{
			Lock::Guard guard(_lock);

			Fs_vfs_handle const *handle = static_cast<Fs_vfs_handle *>(vfs_handle);

			out_count = _write(handle->file_handle(), buf, buf_size, handle->seek());

			return WRITE_OK;
		}

		Read_result read(Vfs_handle *vfs_handle, char *dst, file_size count,
		                 file_size &out_count) override
		{
			Lock::Guard guard(_lock);

			Fs_vfs_handle *handle = static_cast<Fs_vfs_handle *>(vfs_handle);

			/* reset the ready_ready state */
			handle->read_state = Fs_vfs_handle::IDLE;

			out_count = _read(handle->file_handle(), dst, count, handle->seek());

			return READ_OK;
		}

		bool queue_read(Vfs_handle *vfs_handle, file_size count) override
		{
			if (!count)
				Genode::error(__func__,": zero read");
			Lock::Guard guard(_lock);

			Fs_vfs_handle *handle = static_cast<Fs_vfs_handle *>(vfs_handle);

			if (handle->queued_read_state != Fs_vfs_handle::Queued_read::IDLE)
				return false;

			::File_system::Session::Tx::Source &source = *_fs.tx();

			/* if not ready to submit suggest retry */
			if (!source.ready_to_submit()) return false;

			file_size const max_packet_size = source.bulk_buffer_size() / 2;
			file_size const clipped_count = min(max_packet_size, count);

			::File_system::Packet_descriptor p;
			try {
				p = source.alloc_packet(clipped_count);
			} catch (::File_system::Session::Tx::Source::Packet_alloc_failed) {
				return false;
			}

			::File_system::Packet_descriptor const
				packet(p, handle->file_handle(),
				       ::File_system::Packet_descriptor::READ,
				       clipped_count, handle->seek());

			/* pass packet to server side */
			source.submit_packet(packet);

			handle->read_state        = Fs_vfs_handle::IDLE;
			handle->queued_read_state = Fs_vfs_handle::Queued_read::QUEUED;

			return true;
		}

		bool complete_read(Vfs_handle *vfs_handle, char *dst, file_size count,
		                   Read_result &out_result, file_size &out_count) override
		{
			Lock::Guard guard(_lock);

			Fs_vfs_handle *handle = static_cast<Fs_vfs_handle *>(vfs_handle);

			if (handle->queued_read_state != Fs_vfs_handle::Queued_read::ACK)
				return false;

			/* obtain result packet descriptor with updated status info */
			::File_system::Packet_descriptor const
				packet = handle->queued_read_packet;

			file_size const read_num_bytes = min(packet.length(), count);

			::File_system::Session::Tx::Source &source = *_fs.tx();

			memcpy(dst, source.packet_content(packet), read_num_bytes);

			source.release_packet(packet);
			handle->queued_read_state  = Fs_vfs_handle::Queued_read::IDLE;
			handle->queued_read_packet = ::File_system::Packet_descriptor();

			out_result = READ_OK;
			out_count  = read_num_bytes;

			return true;
		}

		Ftruncate_result ftruncate(Vfs_handle *vfs_handle, file_size len) override
		{
			Fs_vfs_handle const *handle = static_cast<Fs_vfs_handle *>(vfs_handle);

			try {
				_fs.truncate(handle->file_handle(), len);
			}
			catch (::File_system::Invalid_handle)    { return FTRUNCATE_ERR_NO_PERM; }
			catch (::File_system::Permission_denied) { return FTRUNCATE_ERR_NO_PERM; }
			catch (::File_system::No_space)          { return FTRUNCATE_ERR_NO_SPACE; }

			return FTRUNCATE_OK;
		}

		bool queue_write(Vfs_handle *vfs_handle, char const *src, file_size count,
		                 Write_result &out_result, file_size &out_count) override
		{
			Lock::Guard guard(_lock);

			Fs_vfs_handle const *handle = static_cast<Fs_vfs_handle *>(vfs_handle);

			::File_system::Session::Tx::Source &source = *_fs.tx();

			/* if not ready to submit suggest retry */
			if (!source.ready_to_submit()) return false;

			file_size const max_packet_size = source.bulk_buffer_size() / 2;
			count = min(max_packet_size, count);

			::File_system::Packet_descriptor p;
			try {
				p = source.alloc_packet(count);
			} catch (::File_system::Session::Tx::Source::Packet_alloc_failed) {
				return false;
			}

			::File_system::Packet_descriptor
				packet(p, handle->file_handle(),
				       ::File_system::Packet_descriptor::WRITE,
				       count, handle->seek());

			memcpy(source.packet_content(packet), src, count);

			/* pass packet to server side */
			source.submit_packet(packet);

			out_count  = count;
			out_result = WRITE_OK;

			return true;
		}

		/****************
		 ** Read_ready **
		 ****************/

		void read_ready_sigh(Genode::Signal_context_capability sigh) override
		{
			_read_ready_sigh = sigh;
		}

		void notify_read_ready(Vfs_handle *vfs_handle) override
		{
			Fs_vfs_handle *handle = static_cast<Fs_vfs_handle *>(vfs_handle);
			if (handle->read_state == Fs_vfs_handle::PENDING
			 || handle->read_state == Fs_vfs_handle::READY)
				return;

			::File_system::Session::Tx::Source &source = *_fs.tx();
			using ::File_system::Packet_descriptor;

			Packet_descriptor packet(Packet_descriptor(),
				                     handle->file_handle(),
			                         Packet_descriptor::READ_READY,
			                         0, 0);

			_fs.tx()->submit_packet(packet);
			handle->read_state = Fs_vfs_handle::PENDING;

			/*
			 * when the packet is acknowledged a signal
			 * will be sent to wake the application
			 */
		}

		bool read_ready(Vfs_handle *vfs_handle) override
		{
			Fs_vfs_handle const *handle =
				static_cast<Fs_vfs_handle *>(vfs_handle);
			return handle->read_state == Fs_vfs_handle::READY;
		}
};

#endif /* _INCLUDE__VFS__FS_FILE_SYSTEM_H_ */
