/*
 * \brief  I/O channel for files opened via virtual directory service
 * \author Norman Feske
 * \date   2011-02-17
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _NOUX__VFS_IO_CHANNEL_H_
#define _NOUX__VFS_IO_CHANNEL_H_

/* Noux includes */
#include <io_channel.h>
#include <vfs/dir_file_system.h>

namespace Noux {
	class Vfs_handle_context;
	struct Vfs_io_channel;
}

class Noux::Vfs_handle_context : public Vfs::Vfs_handle::Context
{
	private:

		Genode::Semaphore _sem;

	public:

		void wait_for_completion()
		{
			_sem.down();
		}

		void complete()
		{
			_sem.up();
		}
};

struct Noux::Vfs_io_channel : Io_channel
{
	Signal_handler<Vfs_io_channel> _read_avail_handler;

	void _handle_read_avail()
	{
		Genode::debug("_handle_read_avail()");
		Io_channel::invoke_all_notifiers();
	}

	Vfs::Vfs_handle *_fh;

	Absolute_path _path;
	Absolute_path _leaf_path;

	Vfs_handle_context _context;

	Vfs_io_channel(char const *path, char const *leaf_path,
	               Vfs::Dir_file_system *root_dir, Vfs::Vfs_handle *vfs_handle,
	               Entrypoint &ep)
	:
		_read_avail_handler(ep, *this, &Vfs_io_channel::_handle_read_avail),
		_fh(vfs_handle), _path(path), _leaf_path(leaf_path)
	{
		Genode::debug("Vfs_io_channel(): ", Genode::Cstring(path));
		_fh->context = &_context;
		_fh->fs().register_read_ready_sigh(_fh, _read_avail_handler);
	}

	~Vfs_io_channel()
	{
		Genode::debug("~Vfs_io_channel(): ", this);
		while (!_fh->ds().sync("/"))
		{
			/* XXX: block until generic IO response */
		}

		_fh->ds().close(_fh);
	}

	bool write(Sysio &sysio, size_t &offset) override
	{
		Genode::debug("Vfs_io_channel::write(): count: ", sysio.write_in.count,
		            ", seek: ", _fh->seek());
		Vfs::file_size count = sysio.write_in.count;
		Vfs::file_size out_count = 0;

		/* XXX: this might not be the best place for the loop */
		do {
			sysio.error.write = _fh->fs().write(_fh, sysio.write_in.chunk,
			                                    count, out_count);
		} while ((out_count == 0) && (count != 0));

//		while (!_fh->ds().sync("/"));

		if (sysio.error.write != Vfs::File_io_service::WRITE_OK)
			return false;
Genode::debug("Vfs_io_channel::write(): out_count: ", out_count);
		_fh->advance_seek(out_count);
Genode::debug("Vfs_io_channel::write(): new seek: ", _fh->seek());

		sysio.write_out.count = out_count;
		offset = out_count;

		return true;
	}

	bool read(Sysio &sysio) override
	{
		Genode::debug("Vfs_io_channel::read()");
		size_t count = min(sysio.read_in.count, sizeof(sysio.read_out.chunk));

		Vfs::file_size out_count = 0;

		if (!_fh->fs().queue_read(_fh, count)) {
			sysio.error.read = Vfs::File_io_service::READ_ERR_INTERRUPT;
			return false;
		}

		for (;;) {

			sysio.error.read = _fh->fs().complete_read(_fh, sysio.read_out.chunk, count, out_count);

			if (sysio.error.read != Vfs::File_io_service::READ_QUEUED)
				break;

			Genode::debug("Vfs_io_channel::read(): blocking for complete_read()");
			_context.wait_for_completion();
			Genode::debug("Vfs_io_channel::read(): unblocked for complete_read()");
		}

		if (sysio.error.read != Vfs::File_io_service::READ_OK)
			return false;

		sysio.read_out.count = out_count;

		_fh->advance_seek(out_count);

		return true;
	}

	bool fstat(Sysio &sysio) override
	{
		/*
		 * 'sysio.stat_in' is not used in '_fh->ds().stat()',
		 * so no 'sysio' member translation is needed here
		 */

		while (!_fh->ds().sync(_leaf_path.base())) {
			/* XXX: block until generic IO response */
		}

		Vfs::Directory_service::Stat stat;

		sysio.error.stat =  _fh->ds().stat(_leaf_path.base(), stat);
		sysio.fstat_out.st = stat;

		return (sysio.error.stat == Vfs::Directory_service::STAT_OK);

	}

	bool ftruncate(Sysio &sysio) override
	{

		sysio.error.ftruncate = _fh->fs().ftruncate(_fh, sysio.ftruncate_in.length);

		return (sysio.error.ftruncate == Vfs::File_io_service::FTRUNCATE_OK);
	}

	bool fcntl(Sysio &sysio) override
	{
		switch (sysio.fcntl_in.cmd) {

		case Sysio::FCNTL_CMD_GET_FILE_STATUS_FLAGS:

			sysio.fcntl_out.result = _fh->status_flags();
			return true;

		case Sysio::FCNTL_CMD_SET_FILE_STATUS_FLAGS:
			_fh->status_flags(sysio.fcntl_in.long_arg);
			return true;

		default:

			warning("invalid fcntl command ", (int)sysio.fcntl_in.cmd);
			sysio.error.fcntl = Sysio::FCNTL_ERR_CMD_INVALID;
			return false;
		};
	}

	/*
	 * The 'dirent' function for the root directory only (the
	 * 'Dir_file_system::open()' function handles all requests referring
	 * to directories). Hence, '_path' is the absolute path of the
	 * directory to inspect.
	 */
	bool dirent(Sysio &sysio) override
	{
		/*
		 * Return artificial dir entries for "." and ".."
		 */
		unsigned const index = _fh->seek() / sizeof(Sysio::Dirent);

		Genode::debug("Vfs_io_channel::dirent(): ", Genode::Cstring(_path.base()),
		            ", index: ", index);

		if (index < 2) {
			sysio.dirent_out.entry.type = Vfs::Directory_service::DIRENT_TYPE_DIRECTORY;
			strncpy(sysio.dirent_out.entry.name,
			        index ? ".." : ".",
			        sizeof(sysio.dirent_out.entry.name));

			sysio.dirent_out.entry.fileno = 1;
			_fh->advance_seek(sizeof(Sysio::Dirent));
			Genode::debug("Vfs_io_channel::dirent(): .. or .");
			return true;
		}

		/*
		 * Delegate remaining dir-entry request to the actual file system.
		 * Align index range to zero when calling the directory service.
		 * The VFS library interprets the seek offset as index.
		 */

		Vfs::Directory_service::Dirent dirent;

		Vfs::file_size noux_dirent_seek = _fh->seek();
		_fh->seek((index - 2) * sizeof(dirent));

		Vfs::file_size const count = sizeof(dirent);

		for (;;) {

			Genode::debug("Vfs_io_channel::dirent(): calling queue_read()");
			if (_fh->fs().queue_read(_fh, count))
				break;

			/*
			 * XXX: block until generic IO response, ideally after
			 *      release_packet()
			 */
		}

		Vfs::File_io_service::Read_result read_result;
		Vfs::file_size out_count = 0;

		for (;;) {

			read_result = _fh->fs().complete_read(_fh, (char*)&dirent,
			                                      count, out_count);

			if (read_result != Vfs::File_io_service::READ_QUEUED)
				break;

			Genode::debug("Vfs_io_channel::dirent(): blocking for complete_read()");
			_context.wait_for_completion();
			Genode::debug("Vfs_io_channel::dirent(): unblocked for complete_read()");
		}

		if ((read_result != Vfs::File_io_service::READ_OK) ||
		    (out_count != sizeof(dirent))) {
		    dirent = Vfs::Directory_service::Dirent();
		}

		Genode::debug("Vfs_io_channel::dirent(): finished");

		_fh->seek(noux_dirent_seek);

		sysio.dirent_out.entry = dirent;

		_fh->advance_seek(sizeof(Sysio::Dirent));
		return true;
	}

	/**
	 * Return size of file that the I/O channel refers to
	 *
	 * Note that this function overwrites the 'sysio' argument. Do not
	 * call it prior saving all input arguments from the original sysio
	 * structure.
	 */
	size_t size(Sysio &sysio)
	{
		if (fstat(sysio))
			return sysio.fstat_out.st.size;

		return 0;
	}

	bool ioctl(Sysio &sysio) override
	{
		Vfs::File_system::Ioctl_arg arg = (Vfs::File_system::Ioctl_arg)sysio.ioctl_in.argp;

		sysio.error.ioctl = _fh->fs().ioctl(_fh, sysio.ioctl_in.request, arg, sysio.ioctl_out);

		return (sysio.error.ioctl == Vfs::File_io_service::IOCTL_OK);
	}

	bool lseek(Sysio &sysio) override
	{
		Genode::debug("Vfs_io_channel::lseek(): ", sysio.lseek_in.offset);
		switch (sysio.lseek_in.whence) {
		case Sysio::LSEEK_SET:
			Genode::debug("Vfs_io_channel::lseek(): LSEEK_SET");
			_fh->seek(sysio.lseek_in.offset); break;
		case Sysio::LSEEK_CUR:
			Genode::debug("Vfs_io_channel::lseek(): LSEEK_CUR");
			_fh->advance_seek(sysio.lseek_in.offset); break;
		case Sysio::LSEEK_END:
			Genode::debug("Vfs_io_channel::lseek(): LSEEK_END");
			off_t offset = sysio.lseek_in.offset;
			sysio.fstat_in.fd = sysio.lseek_in.fd;
			_fh->seek(size(sysio) + offset);
			break;
		}
		sysio.lseek_out.offset = _fh->seek();
		Genode::debug("Vfs_io_channel::lseek(): new seek: ", _fh->seek());
		return true;
	}

	bool check_unblock(bool rd, bool wr, bool ex) const override
	{
		return _fh->fs().check_unblock(_fh, rd, wr, ex);
	}

	bool path(char *path, size_t len) override
	{
		strncpy(path, _path.base(), len);
		path[len - 1] = '\0';

		return true;
	}
};

#endif /* _NOUX__VFS_IO_CHANNEL_H_ */
