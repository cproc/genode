/*
 * \brief  Pipe VFS plugin
 * \author Emery Hemingway
 * \date   2016-09-15
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


/* Genode includes */
#include <vfs/file_system_factory.h>
#include <vfs/file_system.h>
#include <vfs/vfs_handle.h>
#include <util/avl_string.h>
#include <base/debug.h>


namespace Vfs { class Pipe_file_system; };


class Vfs::Pipe_file_system final : public Vfs::File_system
{
	private:

		enum {
			DEFAULT_BUFFER_SIZE = 4096,
			MAX_NAME_LEN = 16
		};

		typedef Genode::String<MAX_NAME_LEN> Pipe_name;

		struct Pipe_handle;
		typedef Genode::List<Pipe_handle> Pipe_handles;

		typedef Genode::Avl_string<MAX_NAME_LEN> Pipe_base;
		struct Pipe : Pipe_base
		{
			Genode::Allocator &alloc; /* allocator of this Pipe */

			Genode::size_t const buf_size;

			Genode::size_t wpos = 0;
			Genode::size_t rpos = 0;

			file_size pending_write = 0;

			char * const buf;

			Pipe_handles write_handles;
			Pipe_handles  read_handles;

			Pipe(Pipe_name const &name, Genode::Allocator &alloc, Genode::size_t buf_size)
			:
				Pipe_base(name.string()),
				alloc(alloc), buf_size(buf_size), buf((char*)alloc.alloc(buf_size))
			{ }

			~Pipe() { alloc.free(buf, buf_size); }

			Genode::size_t read_avail() const
			{
				if (wpos > rpos) return wpos - rpos;
				else             return (wpos - rpos + buf_size) % buf_size;
			}

			Genode::size_t write_avail() const
			{
				if      (wpos > rpos) return ((rpos - wpos + buf_size) % buf_size) - 2;
				else if (wpos < rpos) return rpos - wpos;
				else                  return buf_size - 2;
			}

			Genode::size_t  write(Vfs_handle &handle, Genode::size_t len)
			{
				Genode::size_t const avail = write_avail();
				if (avail == 0) return 0;

				Genode::size_t const limit_len = min(len, avail);
				PDBG("write ",limit_len," of ",len," bytes");
				Genode::size_t const total = wpos + len;
				Genode::size_t first, rest;

				if (total > buf_size) {
					first = buf_size - wpos;
					rest  = total % buf_size;
				} else {
					first = limit_len;
					rest  = 0;
				}

				handle.write_callback(&buf[wpos], first, Callback::PARTIAL);
				wpos = (wpos + first) % buf_size;

				if (rest) {
					handle.read_callback(&buf[wpos], rest, Callback::PARTIAL);
					wpos = (wpos + rest) % buf_size;
				}
				return limit_len;
			}

			Genode::size_t read(Vfs_handle &handle, Genode::size_t len)
			{
				Genode::size_t const avail = read_avail();
				if (avail == 0) return 0;

				Genode::size_t const limit_len = min(len, avail);
				PDBG("read ",limit_len," of ",len," bytes");
				Genode::size_t const total = rpos + len;
				Genode::size_t first, rest;
		
				if (total > buf_size) {
					first = buf_size - rpos;
					rest  = total % buf_size;
				} else {
					first = limit_len;
					rest  = 0;
				}
		
				handle.read_callback(&buf[rpos], first, Callback::PARTIAL);
				rpos = (rpos + first) % buf_size;
		
				if (rest) {
					handle.read_callback(&buf[rpos], rest, Callback::PARTIAL);
					rpos = (rpos + rest) % buf_size;
				}

				return limit_len;
			}
		};

		struct Pipe_handle final : Vfs_handle, Pipe_handles::Element
		{
			Pipe &pipe;

			Pipe_handle(Pipe &pipe, Pipe_file_system &fs,
			            Genode::Allocator &alloc, int flags)
			: Vfs_handle(fs, fs, alloc, flags), pipe(pipe)
			{
				if (flags & OPEN_MODE_WRONLY)
					pipe.write_handles.insert(this);
				else
					pipe.read_handles.insert(this);
			}

			~Pipe_handle() { }
		};

		struct New_pipe_handle final : Vfs_handle
		{
			Pipe_name const name;

			New_pipe_handle(Pipe_name const &pipe_name, Pipe_file_system &fs,
			                Genode::Allocator &alloc, int flags)
			: Vfs_handle(fs, fs, alloc, flags), name(pipe_name) { }

			~New_pipe_handle() { }
		};

		Genode::Avl_tree<Genode::Avl_string_base> _pipes;

		file_size _pipe_count = 0;

		Genode::size_t const _buf_size;

		Pipe *_lookup(char const *name) const
		{
			return static_cast<Pipe *>(_pipes.first() ? 
				_pipes.first()->find_by_name(name) : nullptr);
		}

		Pipe_name _alloc_name()
		{
			static unsigned _pipe_counter = 0;
			/* it overflows, but after a very long time */
			char buf[MAX_NAME_LEN];

			if (_pipe_counter == unsigned(~0U))
				return ""; /* prevent overflow */

			Genode::size_t const n =
				Genode::snprintf(buf, sizeof(buf), "%x", ++_pipe_counter);
			return Pipe_name(Genode::Cstring(buf, n));
		};

	public:

		Pipe_file_system(Genode::Allocator &alloc,
		                 Genode::Xml_node node)
		:
			_buf_size(node.attribute_value("buffer", Genode::size_t(DEFAULT_BUFFER_SIZE)))
		{ }


		/***********************
		 ** Directory service **
		 ***********************/

		Open_result open(char const *path, unsigned mode,
		                 Vfs_handle **handle, Allocator &alloc) override
		{
			if (*path == '/')
				++path;
			/* enforce a flat file system */
			for (char const *p = path; *p; ++p)
				if (*p == '/') return OPEN_ERR_UNACCESSIBLE;

			if (strcmp(path, "new_pipe") == 0) {

				if (mode & OPEN_MODE_CREATE)
					return OPEN_ERR_EXISTS;
					/* cannot create this file */

				if (mode & OPEN_MODE_WRONLY)
					return OPEN_ERR_NO_PERM;
					/* cannot write to this file */

				Pipe_name const pipe_number = _alloc_name();
				if (pipe_number == "")
					return OPEN_ERR_NO_SPACE;
					/* namespace exhausted */

				New_pipe_handle *meta_handle = new (alloc)
					New_pipe_handle(pipe_number, *this, alloc, mode);

				/* the pipe and buffer is allocated from the caller alloc */
				Pipe *pipe;
				try { pipe = new (alloc) Pipe(pipe_number, alloc, _buf_size); }
				catch (Allocator::Out_of_memory) {
					destroy(alloc, meta_handle);
					return OPEN_ERR_NO_SPACE;
				}
				_pipes.insert(pipe);
				++_pipe_count;
				*handle = meta_handle;

				return OPEN_OK;

			} else {

				if (mode & OPEN_MODE_CREATE) {
					return OPEN_ERR_NO_PERM;
					/* cannot manually create pipes */
				}

				if ((mode & OPEN_MODE_ACCMODE) == OPEN_MODE_RDWR) {
					return OPEN_ERR_NO_PERM;
					/* cannot insert a handle into both read and write pipe lists */
				}

				Pipe *pipe = _lookup(path);
				if (!pipe)
					return OPEN_ERR_UNACCESSIBLE;

				*handle = new (alloc)
					Pipe_handle(*pipe, *this, alloc, mode); /* constructor sets R/W handles */

				return OPEN_OK;
			}
		}

		void close(Vfs_handle *vfs_handle) override
		{
			if (Pipe_handle *pipe_handle =
			    dynamic_cast<Pipe_handle *>(vfs_handle)) {
				Pipe &pipe = pipe_handle->pipe;

				if (pipe_handle == pipe.write_handles.first())
					pipe.pending_write = 0;

				pipe.write_handles.remove(pipe_handle);
				pipe.read_handles.remove(pipe_handle);

				destroy(vfs_handle->alloc(), pipe_handle);

			} else if (New_pipe_handle *meta_handle =
			           dynamic_cast<New_pipe_handle *>(vfs_handle)) {
				destroy(vfs_handle->alloc(), meta_handle);
			}
		}

		Unlink_result unlink(char const *path) override
		{
			Pipe *pipe = _lookup(path);
			if (!pipe)
				return UNLINK_ERR_NO_ENTRY;

			/* may not unlink an open pipe */
			if (pipe->write_handles.first() || pipe->read_handles.first())
				return UNLINK_ERR_NO_PERM;

			destroy(pipe->alloc, pipe);
			--_pipe_count;
			return UNLINK_OK;
		}

		Genode::Dataspace_capability dataspace(const char*) override {
			return Genode::Dataspace_capability(); }

		void release(const char*, Genode::Dataspace_capability) override { }

		Stat_result stat(char const *path, Stat &out) override
		{
			out = Stat();
			if (Pipe const *pipe = _lookup(path)) {
				out.size   = pipe->buf_size;
				out.inode  = Genode::addr_t(pipe);
				out.device = Genode::addr_t(this);
				return STAT_OK;
			} else
				return STAT_ERR_NO_ENTRY;
		}

		Dirent_result dirent(char const *, file_offset, Dirent&) override {
			return DIRENT_ERR_NO_PERM; }

		Readlink_result readlink(const char*, char*, file_size, file_size&) override {
			return READLINK_ERR_NO_ENTRY; }

		Rename_result rename(const char *to, const char *from) override {
			return (_lookup(to) || _lookup(from)) ?
				RENAME_ERR_NO_PERM : RENAME_ERR_NO_ENTRY; }

		Mkdir_result mkdir(char const *path, unsigned mode) override {
			return (_lookup(path)) ? MKDIR_ERR_EXISTS : MKDIR_ERR_NO_PERM; }

		Symlink_result symlink(const char*, const char *path) override {
			return (_lookup(path)) ? SYMLINK_ERR_EXISTS : SYMLINK_ERR_NO_PERM; }

		file_size num_dirent(const char*) override
		{
			return 0;
			//return _pipe_count + 1; /* plus 'new_pipe' */
		}

		bool directory(char const *path) override {
			return ((strcmp(path, "") == 0) || (strcmp(path, "/") == 0)); }

		char const *leaf_path(char const *path) override
		{
			if (strcmp(path, "new_pipe") == 0)
				return path;
			return _lookup(path) ? path : nullptr;
		}

		bool subscribe(Vfs_handle *vfs_handle) override {
			return (dynamic_cast<Pipe_handle *>(vfs_handle)); }

		/**********************
		 ** File I/0 service **
		 **********************/

		Write_result write(Vfs_handle *vfs_handle, file_size len, file_size &out) override
		{
			Pipe_handle *write_handle =
				dynamic_cast<Pipe_handle *>(vfs_handle);
			if (!write_handle) {
				return WRITE_ERR_INVALID;
			}

			Pipe &pipe = write_handle->pipe;
			file_size n = pipe.write(*vfs_handle, len);

			file_size remain = len - n;
			if (remain == 0) {
				/* simple, we are done */
				out = n;
				write_handle->write_callback(nullptr, 0, Callback::COMPLETE);
				PDBG("wrote everything in one pass, notifying readers");
				if (Pipe_handle *p = pipe.read_handles.first())
					p->notify_callback();
				return WRITE_OK;

			} else {
				/* not so simple, potentially trigger a callback cascade */
				pipe.pending_write = remain;
				PDBG(remain, " bytes not written, notify reader");
				if (Pipe_handle *p = pipe.read_handles.first())
					p->notify_callback();

				if (pipe.pending_write) {
					PDBG(remain, " bytes not written after notify reader");
					/* pending bytes, so return queued */
					out = n + (remain - pipe.pending_write);
					return WRITE_QUEUED;
				} else {
					PDBG("everything written after notify reader");
					/* the write was completed by read callbacks triggered by notify */
					out = len;
					return WRITE_OK;
				}
			}
		}

		Read_result read(Vfs_handle *vfs_handle, file_size len, file_size &out) override
		{
			if (Pipe_handle *read_handle =
				dynamic_cast<Pipe_handle *>(vfs_handle))
			{
				Pipe &pipe = read_handle->pipe;

				file_size remain = len;
				while(remain) {
					file_size n = pipe.read(*read_handle, len);
					remain -= n;

					if (pipe.pending_write) {
						Pipe_handle &write_handle = *pipe.write_handles.first();
						pipe.pending_write -= pipe.write(write_handle, pipe.pending_write);
						if (pipe.pending_write == 0)
							write_handle.write_callback(nullptr, 0, Callback::COMPLETE);
					} else
						break;
				}

				out = len - remain;
				read_handle->read_callback(nullptr, 0, Callback::COMPLETE);
				if (out)
					if (Pipe_handle *p = pipe.write_handles.first())
						p->notify_callback();

				return READ_OK;

			} else if (New_pipe_handle *meta_handle =
			           dynamic_cast<New_pipe_handle *>(vfs_handle))
			{
				out = meta_handle->name.length()-1;
				meta_handle->read_callback(
					meta_handle->name.string(), out, Callback::COMPLETE);
				/* XXX: a trailing newline would be nice */
				return READ_OK;
			}
			return READ_ERR_INVALID;
		}

		Ftruncate_result ftruncate(Vfs_handle*, file_size len) override {
			return len ? FTRUNCATE_ERR_NO_PERM : FTRUNCATE_OK; }

		
};


extern "C" Vfs::File_system_factory *vfs_file_system_factory(void)
{
	struct Pipe_factory final : Vfs::File_system_factory
	{
		Vfs::File_system *create(Genode::Env &env,
		                         Genode::Allocator &alloc,
		                         Genode::Xml_node node) override
		{
			return new (alloc) Vfs::Pipe_file_system(alloc, node); 
		}
	};

	static Pipe_factory factory;
	return &factory;
}
