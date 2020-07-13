/*
 * \brief  Shared memory file system
 * \author Christian Prochaska
 * \date   2019-03-29
 */

/*
 * Copyright (C) 2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__VFS__SHM_FILE_SYSTEM_H_
#define _INCLUDE__VFS__SHM_FILE_SYSTEM_H_

#include <vfs/file_system.h>
#include <dataspace/client.h>
#include <util/avl_tree.h>

namespace Vfs { class Shm_file_system; }

class Vfs_shm_file : Genode::List<Vfs_shm_file>::Element
{
	private:

		typedef String<MAX_NAME_LEN> Filename;
		Filename       _filename { };

		Ram_allocator &_ram;

		file_size      _length { 0 };

	public:

		bool matches(const char *path)
		{
			return (strlen(path) == (strlen(_filename.string()) + 1)) &&
			       (strcmp(&path[1], _filename.string()) == 0);
		}

		Ram_dataspace_capability ds_cap { };

		Vfs_shm_file(char const *name, Ram_allocator &ram)
		: _filename(name), _ram(ram) { }

		~File()
		{
			if (_length > 0)
				_ram.free(ds_cap);
		}

		size_t read(char *, size_t, file_size) override
		{
			Genode::error(__PRETTY_FUNCTION__);
			return -1;
		}

		Vfs::File_io_service::Read_result complete_read(char *,
		                                                file_size,
		                                                file_size,
			                                            file_size &) override
		{
			return Vfs::File_io_service::READ_ERR_INVALID;
		}

		size_t write(char const *, size_t, file_size) override
		{
			Genode::error(__PRETTY_FUNCTION__);
			return -1;
		}

		file_size length() override { return _length; }

		void truncate(file_size size) override
		{
			if (_length > 0)
				Genode::error(__PRETTY_FUNCTION__, ": resizing not supported yet");

			_length = size;

			ds_cap = _ram.alloc(size);
		}
};


struct Shm_vfs_handle : Vfs_handle
{
	using Vfs_handle::Vfs_handle;

	virtual Read_result read(char *dst, file_size count,
			                 file_size &out_count) = 0;

	virtual Write_result write(char const *src, file_size count,
			                   file_size &out_count) = 0;

	virtual Sync_result sync()
	{
		return SYNC_OK;
	}

	virtual bool read_ready() = 0;
};

struct Shm_vfs_dir_handle : Shm_vfs_handle
{
	private:

		List<Vfs_shm_file> &_files;

		/*
		 * Noncopyable
		 */
		Shm_vfs_dir_handle(Single_vfs_dir_handle const &);
		Shm_vfs_dir_handle &operator = (Shm_vfs_dir_handle const &);

	public:

		Shm_vfs_dir_handle(Directory_service  &ds,
				           File_io_service    &fs,
				           List<Vfs_shm_file> &files,
				           Genode::Allocator  &alloc)
		:
			Shm_vfs_handle(ds, fs, alloc, 0),
			_files(files) { }

		Read_result read(char *dst, file_size count,
				         file_size &out_count) override
		{
			Genode::error("Shm_vfs_dir_handle::read() called, not implemented");

			out_count = 0;

			if (count < sizeof(Dirent))
				return READ_ERR_INVALID;

			Dirent &out = *(Dirent*)dst;

			out = {
				.fileno = (Genode::addr_t)this,
				.type   = Dirent_type::END,
				.rwx    = { },
				.name   = { }
			};

			out_count = sizeof(Dirent);

			return READ_OK;
		}

		Write_result write(char const *, file_size, file_size &) override
		{
			return WRITE_ERR_INVALID;
		}

		bool read_ready() override { return true; }
};

class Vfs::Shm_file_system : public Vfs::File_system
{
	private:

		Vfs::Env &_env;
		Genode::List<Vfs_shm_file> _files;
		size_t                     _num_dirent { 0 };

		bool _root(const char *path)
		{
			return (strcmp(path, "") == 0) || (strcmp(path, "/") == 0);
		}

		Vfs_shm_file *_lookup(char const *path)
		{
			for (Vfs_shm_file *file = _files.first(); file; file = file->next()) {
				if (file->matches(path)
					return file;
			}
			return nullptr;
		}

	public:

		Shm_file_system(Vfs::Env &env, Genode::Xml_node) : _env(env) { }

		~Shm_file_system() { _root.empty(_env.alloc()); }


		/*********************************
		 ** Directory service interface **
		 *********************************/

		Dataspace_capability dataspace(char const *path) override
		{
			using namespace Vfs_shm;
Genode::warning(&path, ": Vfs::Shm_file_system::dataspace(): ", Genode::Cstring(path));
			Ram_dataspace_capability ds_cap;

			Vfs_shm_file *file = _lookup(path);

			if (!file) {
Genode::error("Vfs::Shm_file_system::dataspace(): lookup failed");
				return ds_cap;
			}

			return file->ds_cap;
		}

		void release(char const *, Dataspace_capability ds_cap) override {
			_env.env().ram().free(
				static_cap_cast<Genode::Ram_dataspace>(ds_cap)); }

		Stat_result stat(char const *path, Stat &out) override
		{
			out = Stat { };
			out.device = (Genode::addr_t)this;

			if (_root(path)) {
				out.type = Node_type::DIRECTORY;

			} else if ((Vfs_shm_file *file = _lookup(path))) {
				out.type  = Node_type::CONTINUOUS_FILE;
				out.rwx   = Node_rwx::rw();
				out.inode = (unsigned long)file;
			} else {
				return STAT_ERR_NO_ENTRY;
			}
			return STAT_OK;
		}

		file_size num_dirent(char const *path) override
		{
			if (_root(path))
				return 1;
			else
				return _num_dirent;
		}

		bool directory(char const *path) override
		{
			if (_root(path))
				return true;

			return false;
		}

		char const *leaf_path(char const *path) override {
			return _lookup(path) ? path : nullptr; }

		Opendir_result opendir(char const  *path, bool create,
		                       Vfs_handle **handle,
		                       Allocator   &alloc) override
		{
			if (!_root(path))
				return OPENDIR_ERR_LOOKUP_FAILED;

			if (create)
				return OPENDIR_ERR_PERMISSION_DENIED;

			try {
				*out_handle = new (alloc)
					Shm_vfs_dir_handle(*this, *this, alloc, _files);
				return OPENDIR_OK;
			}
		}

		Open_result open(char const  *path, unsigned mode,
		                 Vfs_handle **handle,
		                 Allocator   &alloc) override
		{
Genode::warning(&path, ": Vfs::Shm_file_system::open(): path: ", Genode::Cstring(path),
                ", mode: ", Genode::Hex(mode));
			using namespace Vfs_shm;

			Vfs_shm_File *file;
			//char const *name = basename(path);
			char const *name = path;
			bool const create = mode & OPEN_MODE_CREATE;

			if (create) {

				/* XXX lookup first */

				if (strlen(name) >= MAX_NAME_LEN)
					return OPEN_ERR_NAME_TOO_LONG;

				try { file = new (_env.alloc()) Vfs_shm_file(name, _env.env().ram()); }
				catch (Out_of_memory) { return OPEN_ERR_NO_SPACE; }

				_files.insert(file);
Genode::warning(&path, ": Vfs::Shm_file_system::open(): new file created: ", file);
			} else {
				Vfs_hm_file *file = _lookup(path);
				if (!file) return OPEN_ERR_UNACCESSIBLE;

Genode::warning(&path, ": Vfs::Shm_file_system::open(): file found: ", file);
			}

			/* XXX create handle */
			try {
				*handle = new (alloc) Io_handle(*this, alloc, mode, *file);
				return OPEN_OK;
			} catch (Genode::Out_of_ram) {
				if (create) {
					lookup_parent(path)->release(file);
					remove(file);
				}
				return OPEN_ERR_OUT_OF_RAM;
			} catch (Genode::Out_of_caps) {
				if (create) {
					lookup_parent(path)->release(file);
					remove(file);
				}
				return OPEN_ERR_OUT_OF_CAPS;
			}
		}

		void close(Vfs_handle *vfs_handle) override
		{
			Vfs_shm::Io_handle *ram_handle =
				static_cast<Vfs_shm::Io_handle *>(vfs_handle);

			Vfs_shm::Node &node = ram_handle->node;
			bool node_modified = ram_handle->modifying;

			ram_handle->node.close(*ram_handle);
			destroy(vfs_handle->alloc(), ram_handle);

			if (ram_handle->node.unlinked() && !ram_handle->node.opened()) {
				destroy(_env.alloc(), &ram_handle->node);
			} else if (node_modified) {
				node.notify();
			}
		}

		Stat_result stat(char const *path, Stat &stat) override
		{
			using namespace Vfs_shm;

			Node *node_ptr = lookup(path);
			if (!node_ptr) return STAT_ERR_NO_ENTRY;

			Node::Guard guard(node_ptr);

			Node &node = *node_ptr;

			auto node_type = [&] ()
			{
				if (dynamic_cast<Directory *>(node_ptr)) return Node_type::DIRECTORY;

				return Node_type::CONTINUOUS_FILE;
			};

			stat = {
				.size              = node.length(),
				.type              = node_type(),
				.rwx               = node.rwx(),
				.inode             = node.inode,
				.device            = (Genode::addr_t)this,
				.modification_time = node.modification_time()
			};

			return STAT_OK;
		}

		Rename_result rename(char const *from, char const *to) override
		{
			return RENAME_ERR_NO_PERM;
		}

		Unlink_result unlink(char const *path) override
		{
			using namespace Vfs_shm;

			Directory *parent = lookup_parent(path);
			if (!parent) return UNLINK_ERR_NO_ENTRY;
			Node::Guard guard(parent);

			Node *node = parent->child(basename(path));
			if (!node) return UNLINK_ERR_NO_ENTRY;

			node->acquire();
			parent->release(node);
			node->notify();
			parent->notify();
			remove(node);
			return UNLINK_OK;
		}



		/************************
		 ** File I/O interface **
		 ************************/

		Write_result write(Vfs_handle *, char const *, file_size,
		                   Vfs::file_size &) override
		{
			return WRITE_ERR_INVALID;
		}

		Read_result complete_read(Vfs_handle *, char *, file_size,
		                          file_size &) override
		{
			return READ_ERR_INVALID;
		}

		bool read_ready(Vfs_handle *) override { return false; }

		Ftruncate_result ftruncate(Vfs_handle *vfs_handle, file_size len) override
		{
Genode::warning(&vfs_handle, ": Vfs::Shm_file_system::ftruncate(): len: ", len);
			if ((vfs_handle->status_flags() & OPEN_MODE_ACCMODE) ==  OPEN_MODE_RDONLY)
				return FTRUNCATE_ERR_NO_PERM;

			Vfs_shm::Io_handle const *handle =
				static_cast<Vfs_shm::Io_handle *>(vfs_handle);

			Vfs_shm::Node::Guard guard(&handle->node);

			try { handle->node.truncate(len); }
			catch (Vfs_shm::Out_of_memory) { return FTRUNCATE_ERR_NO_SPACE; }

			return FTRUNCATE_OK;
		}

		/***************************
		 ** File_system interface **
		 ***************************/

		static char const *name()   { return "shm"; }
		char const *type() override { return "shm"; }
};

#endif /* _INCLUDE__VFS__SHM_FILE_SYSTEM_H_ */
