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

#include <base/log.h>
#include <base/allocator_avl.h>
#include <base/signal.h>
#include <usb/usb.h>
#include <usb_session/connection.h>

#include <vfs/file_system.h>
#include <vfs/vfs_handle.h>

namespace Vfs { class Usb_file_system; }


class Vfs::Usb_file_system : public File_system
{
	public:

		enum Node_type {
			NODE_TYPE_FILE,        NODE_TYPE_SYMLINK,
			NODE_TYPE_CHAR_DEVICE, NODE_TYPE_BLOCK_DEVICE
		};

	private:

		Node_type const _node_type;

		enum { FILENAME_MAX_LEN = 64 };
		char _filename[FILENAME_MAX_LEN];

	protected:

		bool _root(const char *path)
		{
			return (strcmp(path, "") == 0) || (strcmp(path, "/") == 0);
		}

		bool _single_file(const char *path)
		{
			return (strlen(path) == (strlen(_filename) + 1)) &&
			       (strcmp(&path[1], _filename) == 0);
		}

	public:

		Usb_file_system(Xml_node config)
		:
			_node_type(NODE_TYPE_CHAR_DEVICE)
		{
			strncpy(_filename, "state", sizeof(_filename));
		}


		/*********************************
		 ** Directory-service interface **
		 *********************************/

		Dataspace_capability dataspace(char const *path) override
		{
			return Dataspace_capability();
		}

		void release(char const *path, Dataspace_capability ds_cap) override { }

		Stat_result stat(char const *path, Stat &out) override
		{
			out = Stat();
			out.device = (Genode::addr_t)this;

			if (_root(path)) {
				out.mode = STAT_MODE_DIRECTORY;

			} else if (_single_file(path)) {
				switch (_node_type) {
				case NODE_TYPE_FILE:         out.mode = STAT_MODE_FILE;     break;
				case NODE_TYPE_SYMLINK:      out.mode = STAT_MODE_SYMLINK;  break;
				case NODE_TYPE_CHAR_DEVICE:  out.mode = STAT_MODE_CHARDEV;  break;
				case NODE_TYPE_BLOCK_DEVICE: out.mode = STAT_MODE_BLOCKDEV; break;
				}
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
			return _single_file(path) ? path : 0;
		}

		Open_result open(char const  *path, unsigned,
		                 Vfs_handle **out_handle,
		                 Allocator   &alloc) override
		{
			if (!_single_file(path))
				return OPEN_ERR_UNACCESSIBLE;

			*out_handle = new (alloc) Vfs_handle(*this, *this, alloc, 0);
			return OPEN_OK;
		}

		void close(Vfs_handle *handle) override
		{
			if (handle && (&handle->ds() == this))
				destroy(handle->alloc(), handle);
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
			if (_single_file(from) || _single_file(to))
				return RENAME_ERR_NO_PERM;
			return RENAME_ERR_NO_ENTRY;
		}

		Mkdir_result mkdir(char const *, unsigned) override
		{
			return MKDIR_ERR_NO_PERM;
		}

		Symlink_result symlink(char const *, char const *) override
		{
			return SYMLINK_ERR_NO_ENTRY;
		}


		/********************************
		 ** File I/O service interface **
		 ********************************/

		Write_result write(Vfs_handle *handle, char const *, file_size count,
	                   	   file_size &out_count) override
		{
			out_count = 0;

			return WRITE_ERR_INVALID;
		}

		Read_result read(Vfs_handle *vfs_handle, char *dst, file_size count,
	                 	 file_size &out_count) override
		{
			strncpy(dst, "unplugged", count);

			out_count = Genode::max(count, strlen("unplugged") + 1);

			return READ_OK;
		}

		Ftruncate_result ftruncate(Vfs_handle *vfs_handle, file_size) override
		{
			return FTRUNCATE_ERR_NO_PERM;
		}

};

#endif /* _USB_FILE_SYSTEM_H_ */
