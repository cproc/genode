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
#include <base/ram_allocator.h>
#include <dataspace/client.h>
#include <util/list.h>
#include <util/string.h>

namespace Vfs { class Shm_file_system; }


class Vfs::Shm_file_system : public Vfs::File_system
{
	public:

		enum { MAX_NAME_LEN = 128 };

	private:

		class Shm_vfs_file : public Genode::List<Shm_vfs_file>::Element
		{
			private:

				typedef Genode::String<MAX_NAME_LEN> Filename;
				Filename               _filename { };

				Genode::Ram_allocator &_ram;

				Vfs::file_size         _length { 0 };

			public:

				bool matches(const char *path)
				{
					return (Genode::strlen(path) == (Genode::strlen(_filename.string()) + 1)) &&
			       	   	   (Genode::strcmp(&path[1], _filename.string()) == 0);
				}

				Genode::Ram_dataspace_capability ds_cap { };

				Shm_vfs_file(char const *name, Genode::Ram_allocator &ram)
				: _filename(name), _ram(ram) { }

				~Shm_vfs_file()
				{
					if (_length > 0)
						_ram.free(ds_cap);
				}

				Vfs::file_size length() { return _length; }

				Ftruncate_result truncate(Vfs::file_size size)
				{
					if (_length > 0) {
						Genode::error(__PRETTY_FUNCTION__, ": resizing not supported yet");
						return FTRUNCATE_ERR_NO_PERM;
					}

					_length = size;

					ds_cap = _ram.alloc(size);

					return FTRUNCATE_OK;
				}
		};


		class Shm_vfs_handle : public Vfs::Vfs_handle
		{
			public:

				using Vfs::Vfs_handle::Vfs_handle;

				virtual Read_result read(char *dst, file_size count,
				                 	 	 file_size &out_count) = 0;
		};


		class Shm_vfs_dir_handle : public Shm_vfs_handle
		{
			private:

				/*
		 	 	 * Noncopyable
		 	 	 */
				Shm_vfs_dir_handle(Shm_vfs_dir_handle const &);
				Shm_vfs_dir_handle &operator = (Shm_vfs_dir_handle const &);

			public:

				Shm_vfs_dir_handle(Directory_service &ds,
				                   File_io_service   &fs,
				                   Genode::Allocator &alloc)
				: Shm_vfs_handle(ds, fs, alloc, 0) { }

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
		};


		class Shm_vfs_file_handle : public Shm_vfs_handle
		{
			private:
				Shm_vfs_file &_file;

			public:

				Shm_vfs_file_handle(Directory_service &ds,
				                    File_io_service   &fs,
				                    Genode::Allocator &alloc,
				                    Shm_vfs_file &file)
				: Shm_vfs_handle(ds, fs, alloc, 0), _file(file) { }

				Read_result read(char *, file_size, file_size &) override
				{
					return READ_ERR_INVALID;
				}

				Ftruncate_result truncate(file_size len)
				{
					return _file.truncate(len);
				}

		};

		Vfs::Env &_env;
		Genode::List<Shm_vfs_file> _files { };
		Genode::size_t             _num_dirent { 0 };

		bool _root(const char *path)
		{
			return (strcmp(path, "") == 0) || (strcmp(path, "/") == 0);
		}

		Shm_vfs_file *_lookup(char const *path)
		{
			for (Shm_vfs_file *file = _files.first(); file; file = file->next()) {
				if (file->matches(path))
					return file;
			}
			return nullptr;
		}

	public:

		Shm_file_system(Vfs::Env &env, Genode::Xml_node) : _env(env) { }

		~Shm_file_system() { /* XXX */ }


		/*********************************
		 ** Directory service interface **
		 *********************************/

		Dataspace_capability dataspace(char const *path) override
		{
Genode::warning(&path, ": Vfs::Shm_file_system::dataspace(): ", Genode::Cstring(path));
			Ram_dataspace_capability ds_cap;

			Shm_vfs_file *file = _lookup(path);

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

			} else if (_lookup(path)) {
				out.type  = Node_type::CONTINUOUS_FILE;
				out.rwx   = Node_rwx::rw();
				out.inode = (unsigned long)_lookup(path); /* XXX optimize */
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
				*handle = new (alloc) Shm_vfs_dir_handle(*this, *this, alloc);
				return OPENDIR_OK;
			}
			catch (Genode::Out_of_ram)  { return OPENDIR_ERR_OUT_OF_RAM; }
			catch (Genode::Out_of_caps) { return OPENDIR_ERR_OUT_OF_CAPS; }
		}

		Open_result open(char const  *path, unsigned mode,
		                 Vfs_handle **handle,
		                 Allocator   &alloc) override
		{
Genode::warning(&path, ": Vfs::Shm_file_system::open(): path: ", Genode::Cstring(path),
                ", mode: ", Genode::Hex(mode));

			Shm_vfs_file *file { };
			//char const *name = basename(path);
			char const *name = path;
			bool const create = mode & OPEN_MODE_CREATE;

			if (create) {

				/* XXX lookup first */

				if (strlen(name) >= MAX_NAME_LEN)
					return OPEN_ERR_NAME_TOO_LONG;

				try { file = new (_env.alloc()) Shm_vfs_file(name, _env.env().ram()); }
				catch (Allocator::Out_of_memory) { return OPEN_ERR_NO_SPACE; }

				_files.insert(file);
Genode::warning(&path, ": Vfs::Shm_file_system::open(): new file created: ", file);
			} else {
				Shm_vfs_file *file = _lookup(path);
				if (!file) return OPEN_ERR_UNACCESSIBLE;

Genode::warning(&path, ": Vfs::Shm_file_system::open(): file found: ", file);
			}

			try {
				*handle = new (alloc) Shm_vfs_file_handle(*this, *this, alloc, *file);
				return OPEN_OK;
			}
			catch (Genode::Out_of_ram)  { return OPEN_ERR_OUT_OF_RAM; }
			catch (Genode::Out_of_caps) { return OPEN_ERR_OUT_OF_CAPS; }
		}

		void close(Vfs_handle *vfs_handle) override
		{
			if (vfs_handle && (&vfs_handle->ds() == this))
				destroy(vfs_handle->alloc(), vfs_handle);
		}

		Rename_result rename(char const *, char const *) override
		{
			return RENAME_ERR_NO_PERM;
		}

		Unlink_result unlink(char const *) override
		{
			/* XXX */
			Genode::error("Shm_file_system::unlink() called, not implemented");
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
			Shm_vfs_file_handle *handle =
				dynamic_cast<Shm_vfs_file_handle *>(vfs_handle);

			if (!handle)
				return FTRUNCATE_ERR_NO_PERM;

			try { handle->truncate(len); }
			catch (Allocator::Out_of_memory) { return FTRUNCATE_ERR_NO_SPACE; }

			return FTRUNCATE_OK;
		}

		/***************************
		 ** File_system interface **
		 ***************************/

		static char const *name()   { return "shm"; }
		char const *type() override { return "shm"; }
};

#endif /* _INCLUDE__VFS__SHM_FILE_SYSTEM_H_ */
