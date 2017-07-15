/*
 * \brief  Directory file system
 * \author Norman Feske
 * \author Emery Hemingway
 * \author Christian Helmuth
 * \date   2012-04-23
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__VFS__DIR_FILE_SYSTEM_H_
#define _INCLUDE__VFS__DIR_FILE_SYSTEM_H_

#include <vfs/file_system_factory.h>
#include <vfs/vfs_handle.h>


namespace Vfs { class Dir_file_system; }


class Vfs::Dir_file_system : public File_system
{
	public:

		enum { MAX_NAME_LEN = 128 };

	private:

		struct Dir_vfs_handle : Vfs_handle
		{
			Absolute_path  path;
			File_system   *fs_for_complete_read { nullptr };
			Vfs_handle    *fs_dir_handle { nullptr };

			Dir_vfs_handle(Directory_service &ds,
			               File_io_service   &fs,
			               Genode::Allocator &alloc,
			               char const *path)
			: Vfs_handle(ds, fs, alloc, 0),
			  path(path) { }
		};

		/* pointer to first child file system */
		File_system *_first_file_system;

		/* add new file system to the list of children */
		void _append_file_system(File_system *fs)
		{
			if (!_first_file_system) {
				_first_file_system = fs;
				return;
			}

			File_system *curr = _first_file_system;
			while (curr->next)
				curr = curr->next;

			curr->next = fs;
		}

		/**
		 * Directory name
		 */
		char _name[MAX_NAME_LEN];

		bool _root() const { return _name[0] == 0; }

		/**
		 * Perform operation on a file system
		 *
		 * \param fn  functor that takes a file-system reference and
		 *            the path as arguments
		 */
		template <typename RES, typename FN>
		RES _dir_op(RES const no_entry, RES const no_perm, RES const ok,
		            char const *path, FN const &fn)
		{
			Genode::log("_dir_op()");
			path = _sub_path(path);

			/* path does not match directory name */
			if (!path)
				return no_entry;
Genode::log("_dir_op(): check 1");
			/*
			 * Prevent operation if path equals directory name defined
			 * via the static VFS configuration.
			 */
			if (strlen(path) == 0)
				return no_perm;
Genode::log("_dir_op(): check 2");

			/*
			 * If any of the sub file systems returns a permission error and
			 * there exists no sub file system that takes the request, we
			 * return the permission error.
			 */
			bool permission_denied = false;

			/*
			 * Keep the most meaningful error code. When using stacked file
			 * systems, most child file systems will eventually return no
			 * entry (or leave the error code unchanged). If any of those
			 * file systems has anything more interesting to tell, return
			 * this information after all file systems have been tried and
			 * none could handle the request.
			 */
			RES error = ok;

			/*
			 * The given path refers to at least one of our sub directories.
			 * Propagate the request into all of our file systems. If at least
			 * one operation succeeds, we return success.
			 */
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {

				RES const err = fn(*fs, path);

				if (err == ok)
					return err;
Genode::log("_dir_op(): check 3");

				if (err != no_entry && err != no_perm) {
					error = err;
				}

				if (err == no_perm)
					permission_denied = true;
			}
Genode::log("_dir_op(): check 4");

			/* none of our file systems could successfully operate on the path */
			return error != ok ? error : permission_denied ? no_perm : no_entry;
		}

		/**
		 * Return portion of the path after the element corresponding to
		 * the current directory.
		 */
		char const *_sub_path(char const *path) const
		{
			//Genode::log("Dir_file_system::_sub_path(): ", Genode::Cstring(path));
			/* do not strip anything from the path when we are root */
			if (_root())
				return path;

			if (strcmp(path, "/") == 0)
				return path;

			/* skip heading slash in path if present */
			if (path[0] == '/')
				path++;

			Genode::size_t const name_len = strlen(_name);
			if (strcmp(path, _name, name_len) != 0)
				return 0;
			path += name_len;

			/*
			 * The first characters of the first path element are equal to
			 * the current directory name. Let's check if the length of the
			 * first path element matches the name length.
			 */
			if (*path != 0 && *path != '/')
				return 0;

			return path;
		}

		/**
		 * The 'path' is relative to the child file systems.
		 */
		Dirent_result _dirent_of_file_systems(char const *path, file_offset index, Dirent &out)
		{
			int base = 0;
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {

				/*
				 * Determine number of matching directory entries within
				 * the current file system.
				 */
				int const fs_num_dirent = fs->num_dirent(path);

				/*
				 * Query directory entry if index lies with the file
				 * system.
				 */
				if (index - base < fs_num_dirent) {
					index = index - base;
					return fs->dirent(path, index, out);;
				}

				/* adjust base index for next file system */
				base += fs_num_dirent;
			}

			out.type = DIRENT_TYPE_END;
			return DIRENT_OK;
		}

		void _dirent_of_this_dir_node(file_offset index, Dirent &out)
		{
			if (index == 0) {
				strncpy(out.name, _name, sizeof(out.name));

				out.type = DIRENT_TYPE_DIRECTORY;
				out.fileno = 1;
			} else {
				out.type = DIRENT_TYPE_END;
			}
		}

		/*
		 * Accumulate number of directory entries that match in any of
		 * our sub file systems.
		 */
		file_size _sum_dirents_of_file_systems(char const *path)
		{
			file_size cnt = 0;
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {
				cnt += fs->num_dirent(path);
			}
			return cnt;
		}

		bool _queue_read_of_file_systems(Dir_vfs_handle *dir_vfs_handle)
		{
			Genode::log("Dir_file_system::_queue_read_of_file_systems()");

			file_offset index = dir_vfs_handle->seek();

			char const *sub_path = _sub_path(dir_vfs_handle->path.base());

			if (strlen(sub_path) == 0)
				sub_path = "/";

			int base = 0;
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {

				/*
				 * Determine number of matching directory entries within
				 * the current file system.
				 */
				int const fs_num_dirent = fs->num_dirent(sub_path);

				Genode::log("Dir_file_system::_queue_read_of_file_systems(): num: ",
				            fs_num_dirent);

				/*
				 * Query directory entry if index lies with the file
				 * system.
				 */
				if (index - base < fs_num_dirent) {

					Genode::log("Dir_file_system::_queue_read_of_file_systems(): calling opendir() on fs");

					dir_vfs_handle->fs_for_complete_read = fs;

					Opendir_result opendir_result =
						fs->opendir(sub_path, false,
						            &dir_vfs_handle->fs_dir_handle,
						            dir_vfs_handle->alloc());

					/*
					 * Errors of this kind can only be communicated by
					 * 'complete_read()'
					 */
					if (opendir_result != OPENDIR_OK)
						return true;

					dir_vfs_handle->fs_dir_handle->context =
						dir_vfs_handle->context;

					index = index - base;
					dir_vfs_handle->fs_dir_handle->seek(index);

					bool result = fs->queue_read(dir_vfs_handle->fs_dir_handle,
					                             sizeof(Dirent));

					return result;
				}

				/* adjust base index for next file system */
				base += fs_num_dirent;
			}

			Genode::log("Dir_file_system::_queue_read_of_file_systems(): did not find fs for index");

			return true;
		}

		Read_result _complete_read_of_file_systems(Dir_vfs_handle *dir_vfs_handle,
		                                           char *dst, file_size count,
		                                           file_size &out_count)
		{
			if (!dir_vfs_handle->fs_for_complete_read) {
				Genode::log("Dir_file_system::_complete_read_of_file_systems(): no fs found for index");
				/* no fs was found for the given index */
				return READ_ERR_INVALID;
			}

			if (!dir_vfs_handle->fs_dir_handle) {
				Genode::log("Dir_file_system::_complete_read_of_file_systems(): fs->opendir() failed");
				/* fs->opendir() failed */
				return READ_ERR_INVALID;
			}

			Read_result result = dir_vfs_handle->fs_for_complete_read->
			                     complete_read(dir_vfs_handle->fs_dir_handle,
			                                   dst, count, out_count);

			if (result != READ_OK)
				return result;

			dir_vfs_handle->fs_for_complete_read->close(dir_vfs_handle->fs_dir_handle);
			dir_vfs_handle->fs_dir_handle = nullptr;
			dir_vfs_handle->fs_for_complete_read = nullptr;

			return result;
		}

	public:

		Dir_file_system(Genode::Env         &env,
		                Genode::Allocator   &alloc,
		                Genode::Xml_node     node,
		                Io_response_handler &io_handler,
		                File_system_factory &fs_factory)
		:
			_first_file_system(0)
		{
			using namespace Genode;

			/* remember directory name */
			if (node.has_type("fstab") || node.has_type("vfs"))
				_name[0] = 0;
			else
				node.attribute("name").value(_name, sizeof(_name));

			for (unsigned i = 0; i < node.num_sub_nodes(); i++) {

				Xml_node sub_node = node.sub_node(i);

				/* traverse into <dir> nodes */
				if (sub_node.has_type("dir")) {
					_append_file_system(new (alloc)
						Dir_file_system(env, alloc, sub_node, io_handler, fs_factory));
					continue;
				}

				File_system *fs = fs_factory.create(env, alloc, sub_node, io_handler);
				if (fs) {
					_append_file_system(fs);
					continue;
				}

				Genode::error("failed to create <", sub_node.type(), "> VFS node");
				try {
					String<64> value;
					for (unsigned i = 0; i < 16; ++i) {
						Xml_attribute attr = sub_node.attribute(i);
						attr.value(&value);

						Genode::error("\t", attr.name(), "=\"", value, "\"");
					}
				} catch (Xml_node::Nonexistent_attribute) { }
			}
		}


		/*********************************
		 ** Directory-service interface **
		 *********************************/

		Dataspace_capability dataspace(char const *path) override
		{
			path = _sub_path(path);
			if (!path)
				return Dataspace_capability();

			/*
			 * Query sub file systems for dataspace using the path local to
			 * the respective file system
			 */
			File_system *fs = _first_file_system;
			for (; fs; fs = fs->next) {
				Dataspace_capability ds = fs->dataspace(path);
				if (ds.valid())
					return ds;
			}

			return Dataspace_capability();
		}

		void release(char const *path, Dataspace_capability ds_cap) override
		{
			path = _sub_path(path);
			if (!path)
				return;

			for (File_system *fs = _first_file_system; fs; fs = fs->next)
				fs->release(path, ds_cap);
		}

		Stat_result stat(char const *path, Stat &out) override
		{
			//Genode::log("Dir_file_system::stat(): ", Genode::Cstring(path));
			path = _sub_path(path);

			/* path does not match directory name */
			if (!path)
				return STAT_ERR_NO_ENTRY;

			//Genode::log("Dir_file_system::stat(): check");

			/*
			 * If path equals directory name, return information about the
			 * current directory.
			 */
			if (strlen(path) == 0 || (strcmp(path, "/") == 0)) {
				out.size   = 0;
				out.mode   = STAT_MODE_DIRECTORY | 0755;
				out.uid    = 0;
				out.gid    = 0;
				out.inode  = 1;
				out.device = (Genode::addr_t)this;
				return STAT_OK;
			}

			/*
			 * The given path refers to one of our sub directories.
			 * Propagate the request into our file systems.
			 */
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {

				Stat_result const err = fs->stat(path, out);

				if (err == STAT_OK)
					return err;

				if (err != STAT_ERR_NO_ENTRY)
					return err;
			}

			/* none of our file systems felt responsible for the path */
			return STAT_ERR_NO_ENTRY;
		}

		Dirent_result dirent(char const *path, file_offset index, Dirent &out) override
		{
			if (_root())
				return _dirent_of_file_systems(path, index, out);

			if (strcmp(path, "/") == 0) {
				_dirent_of_this_dir_node(index, out);
				return DIRENT_OK;
			}

			/* path contains at least one element */

			/* remove current element from path */
			path = _sub_path(path);

			/* path does not lie within our tree */
			if (!path)
				return DIRENT_ERR_INVALID_PATH;

			return _dirent_of_file_systems(*path ? path : "/", index, out);
		}

		file_size num_dirent(char const *path) override
		{
			Genode::log("Dir_file_system::num_dirent(): self: ", Genode::Cstring(_name));

			Genode::log("Dir_file_system::num_dirent(): path: ", Genode::Cstring(path));

			if (_root()) {

				Genode::log("Dir_file_system::num_dirent(): root");

				return _sum_dirents_of_file_systems(path);

			} else {

				if (strcmp(path, "/") == 0) {
					Genode::log("Dir_file_system::num_dirent(): /");
					return 1;
				}

				/*
				 * The path contains at least one element. Remove current
				 * element from path.
				 */
				path = _sub_path(path);

				Genode::log("Dir_file_system::num_dirent(): sub path: ",
				            Genode::Cstring(path));

				/*
				 * If the resulting 'path' is non-null, the path lies
				 * within our tree. In this case, determine the sum of
				 * matching dirents of all our file systems. Otherwise,
				 * the specified path lies outside our directory node.
				 */
				return path ? _sum_dirents_of_file_systems(*path ? path : "/") : 0;
			}
		}

		/**
		 * Return true if specified path is a directory
		 */
		bool directory(char const *path) override
		{
			//Genode::log("Dir_file_system::directory(): ", Genode::Cstring(path));

			if (strcmp(path, "/") == 0) {
				//Genode::log("Directory_service::directory(): directory");
				return true;
			}

			path = _sub_path(path);
			//Genode::log("Dir_file_system::directory(): sub path: ",
			//            Genode::Cstring(path));
			if (!path) {
				//Genode::log("Directory_service::directory(): not a directory");
				return false;
			}

			if (strlen(path) == 0) {
				//Genode::log("Directory_service::directory(): directory");
				return true;
			}
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {
				//Genode::log("Dir_file_system::directory(): delegating to fs");
				if (fs->directory(path)) {
					//Genode::log("Directory_service::directory(): directory in fs");
					return true;
				}
				//Genode::log("Dir_file_system::directory(): not a directory in fs");
			}
//Genode::log("Dir_file_system::directory(): not a directory in any fs");
			return false;
		}

		/**
		 * Return true if specified path is a directory
		 *
		 * \noapi
		 * \deprecated  use 'directory instead
		 */
		bool is_directory(char const *path) { return directory(path); }

		char const *leaf_path(char const *path) override
		{
			path = _sub_path(path);
			if (!path)
				return 0;

			if (strlen(path) == 0)
				return path;

			for (File_system *fs = _first_file_system; fs; fs = fs->next) {
				char const *leaf_path = fs->leaf_path(path);
				if (leaf_path)
					return leaf_path;
			}

			return 0;
		}

		Open_result open(char const  *path,
		                 unsigned     mode,
		                 Vfs_handle **out_handle,
		                 Allocator   &alloc) override
		{
			Genode::log("Dir_file_system::open(): ", Genode::Cstring(path));

			/*
			 * If 'path' is a directory, we create a 'Vfs_handle'
			 * for the root directory so that subsequent 'dirent' calls
			 * are subjected to the stacked file-system layout.
			 */
			if (directory(path)) {
				*out_handle = new (alloc) Vfs_handle(*this, *this, alloc, 0);
				return OPEN_OK;
			}

			/*
			 * If 'path' refers to a non-directory node, create a
			 * 'Vfs_handle' local to the file system that provides the
			 * file.
			 */

			path = _sub_path(path);

			/* check if path does not match directory name */
			if (!path)
				return OPEN_ERR_UNACCESSIBLE;

			/* path equals directory name */
			if (strlen(path) == 0) {
				*out_handle = new (alloc) Vfs_handle(*this, *this, alloc, 0);
				return OPEN_OK;
			}

			/* path refers to any of our sub file systems */
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {

				Open_result const err = fs->open(path, mode, out_handle, alloc);
				switch (err) {
				case OPEN_ERR_UNACCESSIBLE:
					continue;
				default:
					return err;
				}
			}

			/* path does not match any existing file or directory */
			return OPEN_ERR_UNACCESSIBLE;
		}

		Opendir_result opendir(char const *path, bool create,
		                       Vfs_handle **out_handle, Allocator &alloc) override
		{
			Genode::log("Dir_file_system::opendir(): self: ", Genode::Cstring(_name));

			Genode::log("Dir_file_system::opendir(): path: ", Genode::Cstring(path));

			/* path equals "/" (for reading the name of this directory) */
			if (strcmp(path, "/") == 0) {
				Genode::log("Dir_file_system::opendir(): found");
				if (create)
					return OPENDIR_ERR_PERMISSION_DENIED;
				*out_handle = new (alloc) Dir_vfs_handle(*this, *this, alloc,
				                                         path);
				return OPENDIR_OK;
			}

			char const *sub_path = _sub_path(path);

			/* path equals directory name (for reading the fs directory entries) */
			if (sub_path && (strlen(sub_path) == 0)) {
				Genode::log("Dir_file_system::opendir(): found");
				if (create)
					return OPENDIR_ERR_PERMISSION_DENIED;
				*out_handle = new (alloc) Dir_vfs_handle(*this, *this, alloc,
				                                         _name);
				return OPENDIR_OK;
			}

			/* forward to file systems */

			Genode::log("Dir_file_system::opendir(): delegating to fs");

			auto opendir_fn = [&] (File_system &fs, char const *path)
			{
				return fs.opendir(path, create, out_handle, alloc);
			};

			return _dir_op(OPENDIR_ERR_LOOKUP_FAILED,
			               OPENDIR_ERR_PERMISSION_DENIED,
			               OPENDIR_OK,
			               path, opendir_fn);
		}

		Openlink_result openlink(char const *path, bool create,
		                         Vfs_handle **out_handle,
		                         Allocator &alloc) override
		{
			Genode::log("Dir_file_system::openlink()");

			auto openlink_fn = [&] (File_system &fs, char const *path)
			{
				return fs.openlink(path, create, out_handle, alloc);
			};

			return _dir_op(OPENLINK_ERR_LOOKUP_FAILED,
			               OPENLINK_ERR_PERMISSION_DENIED,
			               OPENLINK_OK,
			               path, openlink_fn);
		}

		void close(Vfs_handle *handle) override
		{
			if (handle && (&handle->ds() == this))
				destroy(handle->alloc(), handle);
		}

		Unlink_result unlink(char const *path) override
		{
			auto unlink_fn = [] (File_system &fs, char const *path)
			{
				return fs.unlink(path);
			};

			return _dir_op(UNLINK_ERR_NO_ENTRY, UNLINK_ERR_NO_PERM, UNLINK_OK,
			               path, unlink_fn);
		}

		Readlink_result readlink(char const *path, char *buf, file_size buf_size,
		                         file_size &out_len) override
		{
			auto readlink_fn = [&] (File_system &fs, char const *path)
			{
				return fs.readlink(path, buf, buf_size, out_len);
			};

			return _dir_op(READLINK_ERR_NO_ENTRY, READLINK_ERR_NO_ENTRY, READLINK_OK,
			               path, readlink_fn);
		}

		Rename_result rename(char const *from_path, char const *to_path) override
		{
			from_path = _sub_path(from_path);
			to_path = _sub_path(to_path);

			/* path does not match directory name */
			if (!from_path)
				return RENAME_ERR_NO_ENTRY;

			/*
			 * Cannot rename a path in the static VFS configuration.
			 */
			if (strlen(from_path) == 0)
				return RENAME_ERR_NO_PERM;

			/*
			 * Check if destination path resides within the same file
			 * system instance as the source path.
			 */
			if (!to_path)
				return RENAME_ERR_CROSS_FS;

			Rename_result final = RENAME_ERR_NO_ENTRY;
			for (File_system *fs = _first_file_system; fs; fs = fs->next) {
				switch (fs->rename(from_path, to_path)) {
				case RENAME_OK:           return RENAME_OK;
				case RENAME_ERR_NO_ENTRY: continue;
				case RENAME_ERR_NO_PERM:  return RENAME_ERR_NO_PERM;
				case RENAME_ERR_CROSS_FS: final = RENAME_ERR_CROSS_FS;
				}
			}
			return final;
		}

		Symlink_result symlink(char const *from, char const *to) override
		{
			auto symlink_fn = [&] (File_system &fs, char const *to)
			{
				return fs.symlink(from, to);
			};

			return _dir_op(SYMLINK_ERR_NO_ENTRY, SYMLINK_ERR_NO_PERM, SYMLINK_OK,
			               to, symlink_fn);
		}

		Mkdir_result mkdir(char const *path, unsigned mode) override
		{
#if 0
			Opendir_result opendir_result = opendir(path, true);

			switch (opendir_result) {
				case OPENDIR_OK:                      return MKDIR_OK;
				case OPENDIR_ERR_LOOKUP_FAILED:       return MKDIR_ERR_NO_ENTRY;
				case OPENDIR_ERR_NAME_TOO_LONG:       return MKDIR_ERR_NAME_TOO_LONG;
				case OPENDIR_ERR_NODE_ALREADY_EXISTS: return MKDIR_ERR_EXISTS;
				case OPENDIR_ERR_NO_SPACE:            return MKDIR_ERR_NO_SPACE;
				case OPENDIR_ERR_PERMISSION_DENIED:   return MKDIR_ERR_NO_PERM;
			}
#endif
			
			auto mkdir_fn = [&] (File_system &fs, char const *path)
			{
				return fs.mkdir(path, mode);
			};

			return _dir_op(MKDIR_ERR_NO_ENTRY, MKDIR_ERR_NO_PERM, MKDIR_OK,
			               path, mkdir_fn);
		}


		/***************************
		 ** File_system interface **
		 ***************************/

		char const *name() const    { return "dir"; }
		char const *type() override { return "dir"; }

		/**
		 * Synchronize all file systems
		 */
		void sync(char const *path) override
		{
			if (strcmp("/", path, 2) == 0) {
				for (File_system *fs = _first_file_system; fs; fs = fs->next)
					fs->sync("/");
				return;
			}

			path = _sub_path(path);
			if (!path)
				return;

			for (File_system *fs = _first_file_system; fs; fs = fs->next)
				fs->sync(path);
		}

		void apply_config(Genode::Xml_node const &node) override
		{
			using namespace Genode;

			File_system *curr = _first_file_system;
			for (unsigned i = 0; i < node.num_sub_nodes(); i++, curr = curr->next) {
				Xml_node const &sub_node = node.sub_node(i);

				/* check if type of XML node matches current file-system type */
				if (sub_node.has_type(curr->type()) == false) {
					Genode::error("VFS config update failed (node type '",
					               sub_node.type(), "' != fs type '", curr->type(),"')");
					return;
				}

				curr->apply_config(node.sub_node(i));
			}
		}


		/********************************
		 ** File I/O service interface **
		 ********************************/

		Write_result write(Vfs_handle *handle, char const *, file_size,
		                   file_size &) override
		{
			return WRITE_ERR_INVALID;
		}

		Read_result read(Vfs_handle *vfs_handle, char *dst,
		                 file_size count, file_size &out_count) override
		{
			Genode::log("Dir_file_system::read(): ", vfs_handle->seek());
			if (count < sizeof(Dirent))
				return READ_ERR_INVALID;

			Dirent *dirent = (Dirent*)dst;
			file_offset index = vfs_handle->seek();

			if (index == 0) {
				strncpy(dirent->name, _name, sizeof(dirent->name));

				dirent->type = DIRENT_TYPE_DIRECTORY;
				dirent->fileno = 1;
			} else {
				dirent->type = DIRENT_TYPE_END;
			}
			Genode::log("Dir_file_system::read(): ", Genode::Cstring(dirent->name));

			return READ_OK;
		}

		bool queue_read(Vfs_handle *vfs_handle, file_size count) override
		{
			Dir_vfs_handle *dir_vfs_handle =
				static_cast<Dir_vfs_handle*>(vfs_handle);

			Genode::log("Dir_file_system::queue_read(): self: ",
			            Genode::Cstring(_name));

			Genode::log("Dir_file_system::queue_read(): path: ",
			            Genode::Cstring(dir_vfs_handle->path.base()),
			            ", index: ", dir_vfs_handle->seek());

			if (_root()) {
				Genode::log("Dir_file_system::queue_read(): root, delegating to fs");
				return _queue_read_of_file_systems(dir_vfs_handle);
			}

			if (strcmp(dir_vfs_handle->path.base(), "/") == 0) {
				Genode::log("Dir_file_system::queue_read(): /, queued");
				return true;
			}

			Genode::log("Dir_file_system::queue_read(): delegating to fs");

			return _queue_read_of_file_systems(dir_vfs_handle);
		}

		Read_result complete_read(Vfs_handle *vfs_handle,
	                              char *dst, file_size count,
	                              file_size &out_count) override
	    {
			out_count = 0;

			if (count < sizeof(Dirent))
				return READ_ERR_INVALID;

			Dir_vfs_handle *dir_vfs_handle =
				static_cast<Dir_vfs_handle*>(vfs_handle);

			Genode::log("Dir_file_system::complete_read(): self: ",
			            Genode::Cstring(_name));

			Genode::log("Dir_file_system::complete_read(): path: ",
			            Genode::Cstring(dir_vfs_handle->path.base()),
			            ", index: ", dir_vfs_handle->seek());

			if (_root()) {
				Genode::log("Dir_file_system::complete_read(): root, delegating...");
				return _complete_read_of_file_systems(dir_vfs_handle, dst, count, out_count);
			}

			if (strcmp(dir_vfs_handle->path.base(), "/") == 0) {
				Genode::log("Dir_file_system::complete_read(): /");

				Dirent *dirent = (Dirent*)dst;
				file_offset index = vfs_handle->seek();

				if (index == 0) {
					strncpy(dirent->name, _name, sizeof(dirent->name));

					dirent->type = DIRENT_TYPE_DIRECTORY;
					dirent->fileno = 1;
				} else {
					dirent->type = DIRENT_TYPE_END;
				}

				out_count = sizeof(Dirent);

				return READ_OK;
			}

			Genode::log("Dir_file_system::complete_read(): delegating to fs");
			return _complete_read_of_file_systems(dir_vfs_handle, dst, count, out_count);
	    }

		Ftruncate_result ftruncate(Vfs_handle *, file_size) override
		{
			return FTRUNCATE_ERR_NO_PERM;
		}

		bool read_ready(Vfs_handle *handle) override
		{
			if (&handle->fs() == this)
				return true;

			return handle->fs().read_ready(handle);
		}

		bool notify_read_ready(Vfs_handle *handle) override
		{
			if (&handle->fs() == this)
				return true;

			return handle->fs().notify_read_ready(handle);
		}
};

#endif /* _INCLUDE__VFS__DIR_FILE_SYSTEM_H_ */
