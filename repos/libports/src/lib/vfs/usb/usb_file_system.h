/*
 * \brief  USB file system
 * \author Christian Prochaska
 * \date   2016-11-02
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _USB_FILE_SYSTEM_H_
#define _USB_FILE_SYSTEM_H_

/* Genode includes */
#include <base/log.h>
#include <base/allocator_avl.h>
#include <base/signal.h>
#include <usb/usb.h>
#include <usb_session/connection.h>
#include <vfs/file_system.h>
#include <vfs/vfs_handle.h>

/* libc includes */
#include <string.h>

/* libusb includes */
#include <libusb.h>

namespace Vfs { class Usb_file_system; }


class Vfs::Usb_file_system : public File_system
{
	private:

		enum State { UNPLUGGED = 0, PLUGGED = 1};

		Genode::Env             &_env;
		Genode::Allocator       &_alloc;

		Genode::Allocator_avl    _usb_tx_block_alloc { &_alloc };

		Genode::Signal_handler<Usb_file_system> _state_change_signal_handler;

		Usb::Connection _usb_connection { _env,
		                                  &_usb_tx_block_alloc,
		                                  "usb_device",
		                                  512*1024,
		                                  _state_change_signal_handler };

		Usb::Device_descriptor    _device_descriptor;
		Usb::Config_descriptor    _config_descriptor;
		Usb::Interface_descriptor _interface_descriptor;

		State                     _state { UNPLUGGED };

		class Usb_file_handle : public Vfs_handle
		{
			public:

				using Vfs_handle::Vfs_handle;

				virtual void subscribe() = 0;
				virtual void unsubscribe() = 0;
				virtual void notify() = 0;

				virtual Read_result read(file_size count, file_size &out_count) = 0;
				virtual Write_result write(file_size count, file_size &out_count) = 0;
		};

		class State_file_handle final : public Usb_file_handle,
		                                public Genode::List<State_file_handle>::Element
		{
			private:

				Genode::List<State_file_handle> &_subscribers;
				State                           &_state;

				char const *_state_string[2] = { "unplugged", "plugged" };

			public:

				State_file_handle(Directory_service &ds,
				                  File_io_service   &fs,
				                  Genode::Allocator &alloc,
				                  int status_flags,
				                  Genode::List<State_file_handle> &subscribers,
				                  State &state)
				: Usb_file_handle(ds, fs, alloc, status_flags),
				  _subscribers(subscribers),
				  _state(state) { }

				void subscribe()
				{
					for (State_file_handle *h = _subscribers.first(); h; h = h->next())
						if (h == this) return;

					_subscribers.insert(this);
				}

				void unsubscribe() { _subscribers.remove(this); }

				void notify() { notify_callback(); }

				Read_result read(file_size count, file_size &out_count)
				{
					/* XXX: take seek pointer into account */

					out_count = read_callback(_state_string[_state], count, Callback::COMPLETE);

					return READ_OK;
				}

				Write_result write(file_size count, file_size &out_count)
				{
					out_count = 0;
					return WRITE_ERR_INVALID;
				}
		};

		Genode::List<State_file_handle> _state_file_subscribers;

		class Ep_file_handle final : public Usb_file_handle,
		                             public Genode::List<Ep_file_handle>::Element
		{
			private:

				Genode::List<Ep_file_handle> &_subscribers;

				Usb::Connection              &_usb_connection;



			public:

				Ep_file_handle(Directory_service &ds,
				               File_io_service   &fs,
				               Genode::Allocator &alloc,
				               int status_flags,
				               Genode::List<Ep_file_handle> &subscribers,
				               Usb::Connection &usb_connection)
				: Usb_file_handle(ds, fs, alloc, status_flags),
				  _subscribers(subscribers),
				  _usb_connection(usb_connection) { }

				void subscribe()
				{
					for (Ep_file_handle *h = _subscribers.first(); h; h = h->next())
						if (h == this) return;

					_subscribers.insert(this);
				}

				void unsubscribe() { _subscribers.remove(this); }

				void notify() { notify_callback(); }

				Read_result read(file_size count, file_size &out_count)
				{
					/* XXX: take seek pointer into account */

					out_count = read_callback("test", count, Callback::COMPLETE);

					return READ_OK;
				}

				Write_result write(file_size count, file_size &out_count)
				{
					Genode::log(__PRETTY_FUNCTION__);
					out_count = 0;
					return WRITE_ERR_INVALID;
				}
		};

		Genode::List<Ep_file_handle> _ep_file_subscribers;

		enum Node_type {
			NODE_TYPE_FILE,        NODE_TYPE_SYMLINK,
			NODE_TYPE_CHAR_DEVICE, NODE_TYPE_BLOCK_DEVICE
		};

		Node_type const _node_type;

		enum { FILENAME_MAX_LEN = 64 };
		char _filename[FILENAME_MAX_LEN];

		bool _root(const char *path)
		{
			return (strcmp(path, "") == 0) || (strcmp(path, "/") == 0);
		}

		bool _state_file(const char *path)
		{
			return (strlen(path) == strlen("/state")) &&
			       (strcmp(path, "/state") == 0);
		}

		bool _ep_file(const char *path)
		{
			/* currently, only /ep0 .. /ep9 is supported */
			return (strlen(path) == strlen("/ep") + 1) &&
			       (strncmp(path, "/ep", 3) == 0) &&
			       (path[3] >= '0') && (path[3] <= '9');
		}

		void _handle_state_change()
		{
			Genode::log("handle_state_change()");

			if (_usb_connection.plugged()) {
				_state = PLUGGED;

				_usb_connection.config_descriptor(&_device_descriptor,
				                                  &_config_descriptor);

				_usb_connection.interface_descriptor(0, 0, &_interface_descriptor);

				Genode::log("num_endpoints: ", _interface_descriptor.num_endpoints);

			} else
				_state = UNPLUGGED;

			for (State_file_handle *h = _state_file_subscribers.first(); h; h = h->next())
				h->notify();
		}

	public:

		Usb_file_system(Genode::Env &env,
		                Genode::Allocator &alloc,
		                Genode::Xml_node config)
		:
			_env(env),
			_alloc(alloc),
			_state_change_signal_handler(_env.ep(), *this,
			                             &Usb_file_system::_handle_state_change),
			_node_type(NODE_TYPE_CHAR_DEVICE)
		{
			strncpy(_filename, "state", sizeof(_filename));
			Genode::log("USB file system constructed");
		}


		/*********************************
		 ** Directory-service interface **
		 *********************************/

		Dataspace_capability dataspace(char const *path) override
		{
			return Dataspace_capability();
		}

		void release(char const *path, Dataspace_capability ds_cap) override { }

		Open_result open(char const  *path, unsigned,
		                 Vfs_handle **out_handle,
		                 Allocator   &alloc) override
		{
			if (_state_file(path))
				*out_handle = new (alloc) State_file_handle(*this, *this, alloc, 0,
			                                                _state_file_subscribers,
			                                                _state);
			else if (_ep_file(path))
				*out_handle = new (alloc) Ep_file_handle(*this, *this, alloc, 0,
			                                             _ep_file_subscribers,
			                                             _usb_connection);
			else
				return OPEN_ERR_UNACCESSIBLE;

			return OPEN_OK;
		}

		void close(Vfs_handle *vfs_handle) override
		{
			if (vfs_handle && (&vfs_handle->ds() == this))
				destroy(vfs_handle->alloc(), vfs_handle);
		}

		Stat_result stat(char const *path, Stat &out) override
		{
Genode::log("stat(): ", path);
			out = Stat();
			out.device = (Genode::addr_t)this;

			if (_root(path)) {
				out.mode = STAT_MODE_DIRECTORY;

			} else if (_state_file(path)) {
				out.mode = STAT_MODE_FILE;
				out.inode = 1;
			} else if (_ep_file(path)) {
				out.mode = STAT_MODE_CHARDEV;
				out.inode = 1;
			} else {
				return STAT_ERR_NO_ENTRY;
			}
			return STAT_OK;
		}

		Dirent_result dirent(char const *path, file_offset index, Dirent &out) override
		{
			if (!_root(path))
				return DIRENT_ERR_INVALID_PATH;

			if (index == 0) {
				out.fileno = (Genode::addr_t)this;
				switch (_node_type) {
				case NODE_TYPE_FILE:         out.type = DIRENT_TYPE_FILE;     break;
				case NODE_TYPE_SYMLINK:      out.type = DIRENT_TYPE_SYMLINK;  break;
				case NODE_TYPE_CHAR_DEVICE:  out.type = DIRENT_TYPE_CHARDEV;  break;
				case NODE_TYPE_BLOCK_DEVICE: out.type = DIRENT_TYPE_BLOCKDEV; break;
				}
				strncpy(out.name, _filename, sizeof(out.name));
			} else {
				out.type = DIRENT_TYPE_END;
			}

			return DIRENT_OK;
		}

		Unlink_result unlink(char const *) override
		{
			return UNLINK_ERR_NO_PERM;
		}

		Readlink_result readlink(char const *, char *, file_size,
		                         file_size &) override
		{
			return READLINK_ERR_NO_ENTRY;
		}

		Rename_result rename(char const *from, char const *to) override
		{
			return RENAME_ERR_NO_PERM;
		}

		Mkdir_result mkdir(char const *, unsigned) override
		{
			return MKDIR_ERR_NO_PERM;
		}

		Symlink_result symlink(char const *, char const *) override
		{
			return SYMLINK_ERR_NO_ENTRY;
		}

		file_size num_dirent(char const *path) override
		{
			if (_root(path))
				return 1;
			else
				return 0;
		}

		bool directory(char const *path) override
		{
			if (_root(path))
				return true;

			return false;
		}

		char const *leaf_path(char const *path) override
		{
			if (_state_file(path) || _ep_file(path))
				return path;
				
			return 0;
		}

		bool subscribe(Vfs::Vfs_handle *vfs_handle) override
		{
			Usb_file_handle *handle = static_cast<Usb_file_handle*>(vfs_handle);
			handle->subscribe();
			return true;
		}

		/********************************
		 ** File I/O service interface **
		 ********************************/

		Write_result write(Vfs_handle *vfs_handle, file_size count,
	                   	   file_size &out_count) override
		{
			/* XXX: take seek pointer into account */

			if (!vfs_handle)
				return WRITE_ERR_INVALID;

			Usb_file_handle *handle = static_cast<Usb_file_handle *>(vfs_handle);

			return handle->write(count, out_count);
		}

		Read_result read(Vfs_handle *vfs_handle, file_size count,
	                 	 file_size &out_count) override
		{
			/* XXX: take seek pointer into account */

			if (!vfs_handle)
				return READ_ERR_INVALID;

			Usb_file_handle *handle = static_cast<Usb_file_handle *>(vfs_handle);

			return handle->read(count, out_count);
		}

		Ftruncate_result ftruncate(Vfs_handle *vfs_handle, file_size) override
		{
			return FTRUNCATE_ERR_NO_PERM;
		}

};

#endif /* _USB_FILE_SYSTEM_H_ */
