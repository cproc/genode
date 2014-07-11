/*
 * \brief  Pipe plugin implementation
 * \author Christian Prochaska
 * \date   2014-07-11
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/env.h>
#include <base/printf.h>
#include <os/ring_buffer.h>
#include <util/misc_math.h>

/* libc includes */
#include <fcntl.h>
#include <stdlib.h>

/* libc plugin interface */
#include <libc-plugin/fd_alloc.h>
#include <libc-plugin/plugin_registry.h>
#include <libc-plugin/plugin.h>


/* function to notify libc about a socket event */
extern void (*libc_select_notify)();


namespace Libc_pipe {


	enum Type { READ_END, WRITE_END };
	enum { PIPE_BUF_SIZE = 4096 };

	typedef Ring_buffer<unsigned char, PIPE_BUF_SIZE+1> Pipe_buffer;

	class Plugin_context : public Libc::Plugin_context
	{
		private:

			Type _type;

			Pipe_buffer *_buffer;

			Libc::File_descriptor *_partner;
			Genode::Lock *_write_avail_lock;
			Genode::Cancelable_lock::State *_lock_state;

		public:

			/**
			 * Constructor
			 *
			 * \param type     information if the file descriptor belongs to the
			 *                 read end or to the write end of the pipe
			 *
			 * \param partner  the other pipe end
			 */
			Plugin_context(Type type, Libc::File_descriptor *partner);

			~Plugin_context();

			Type type() const                            { return _type; }
			Pipe_buffer *buffer() const                  { return _buffer; }
			Libc::File_descriptor *partner() const       { return _partner; }
			Genode::Lock *write_avail_lock() const       { return _write_avail_lock; }

			void set_partner(Libc::File_descriptor *partner)
			{
				_partner = partner;
			}
	};


	class Plugin : public Libc::Plugin
	{
		public:

			/**
			 * Constructor
			 */
			Plugin();

			bool supports_pipe();
			bool supports_select(int nfds,
			                     fd_set *readfds,
			                     fd_set *writefds,
			                     fd_set *exceptfds,
			                     struct timeval *timeout);

			int close(Libc::File_descriptor *pipefdo);
			int fcntl(Libc::File_descriptor *pipefdo, int cmd, long arg);
			int pipe(Libc::File_descriptor *pipefdo[2]);
			ssize_t read(Libc::File_descriptor *pipefdo, void *buf, ::size_t count);
			int select(int nfds, fd_set *readfds, fd_set *writefds,
			           fd_set *exceptfds, struct timeval *timeout);
			ssize_t write(Libc::File_descriptor *pipefdo, const void *buf, ::size_t count);
	};


	/***************
	 ** Utilities **
	 ***************/

	Plugin_context *context(Libc::File_descriptor *fd)
	{
		return static_cast<Plugin_context *>(fd->context);
	}


	static inline bool is_read_end(Libc::File_descriptor *fdo)
	{
		return (context(fdo)->type() == READ_END);
	}


	static inline bool is_write_end(Libc::File_descriptor *fdo)
	{
		return (context(fdo)->type() == WRITE_END);
	}


	/********************
	 ** Plugin_context **
	 ********************/

	Plugin_context::Plugin_context(Type type, Libc::File_descriptor *partner)
	: _type(type), _partner(partner)
	{
		if (!_partner) {

			/* allocate shared resources */

			_buffer = new (Genode::env()->heap()) Pipe_buffer;
			_write_avail_lock = new (Genode::env()->heap()) Genode::Lock;

		} else {

			/* get shared resource pointers from partner */

			_buffer     = context(_partner)->buffer();
			_write_avail_lock = context(_partner)->write_avail_lock();
		}
	}


	Plugin_context::~Plugin_context()
	{
		if (_partner) {
			/* remove the fd this context belongs to from the partner's context */
			context(_partner)->set_partner(0);
		} else {
			/* partner fd is already destroyed -> free shared resources */
			destroy(Genode::env()->heap(), _buffer);
			destroy(Genode::env()->heap(), _write_avail_lock);
		}
	}


	/************
	 ** Plugin **
	 ************/

	Plugin::Plugin()
	{
		Genode::printf("using the pipe libc plugin\n");
	}


	bool Plugin::supports_pipe()
	{
		return true;
	}


	bool Plugin::supports_select(int nfds,
	                             fd_set *readfds,
	                             fd_set *writefds,
	                             fd_set *exceptfds,
	                             struct timeval *timeout)
	{
		/*
		 * Return true if any file descriptor which is marked set in one of
		 * the sets belongs to this plugin
		 */
		for (int libc_fd = 0; libc_fd < nfds; libc_fd++) {
			if (FD_ISSET(libc_fd, readfds) || FD_ISSET(libc_fd, writefds)
			 || FD_ISSET(libc_fd, exceptfds)) {
				Libc::File_descriptor *fdo =
					Libc::file_descriptor_allocator()->find_by_libc_fd(libc_fd);
				if (fdo && (fdo->plugin == this)) {
					return true;
				}
			}
		}
		return false;
	}


	int Plugin::close(Libc::File_descriptor *pipefdo)
	{
		Genode::destroy(Genode::env()->heap(), context(pipefdo));
		Libc::file_descriptor_allocator()->free(pipefdo);

		return 0;
	}


	int Plugin::fcntl(Libc::File_descriptor *pipefdo, int cmd, long arg)
	{
		switch (cmd) {
			case F_GETFL:
				if (is_write_end(pipefdo))
					return O_WRONLY;
				else
					return O_RDONLY;
			default: PERR("fcntl(): command %d not supported", cmd); return -1;
		}
	}


	int Plugin::pipe(Libc::File_descriptor *pipefdo[2])
	{
		pipefdo[0] = Libc::file_descriptor_allocator()->alloc(this,
		               new (Genode::env()->heap()) Plugin_context(READ_END, 0));
		pipefdo[1] = Libc::file_descriptor_allocator()->alloc(this,
		               new (Genode::env()->heap()) Plugin_context(WRITE_END, pipefdo[0]));
		static_cast<Plugin_context *>(pipefdo[0]->context)->set_partner(pipefdo[1]);

		return 0;
	}


	ssize_t Plugin::read(Libc::File_descriptor *fdo, void *buf, ::size_t count)
	{
		/* TODO: thread-safety */
		/* TODO: non-blocking mode */

		if (!is_read_end(fdo)) {
			PERR("Cannot read from write end of pipe.");
			return -1;
		}

		for (::size_t num_bytes_read = 0;
		     num_bytes_read < count;
		     num_bytes_read++) {
			((unsigned char*)buf)[num_bytes_read] = context(fdo)->buffer()->get();
			context(fdo)->write_avail_lock()->unlock();
		}

		return count;
	}


	/* no support for execptfds right now */
	int Plugin::select(int nfds,
	                   fd_set *readfds,
	                   fd_set *writefds,
	                   fd_set *exceptfds,
	                   struct timeval *timeout)
	{
		int nready = 0;
		Libc::File_descriptor *fdo;
		fd_set in_readfds, in_writefds;

		in_readfds = *readfds;
		FD_ZERO(readfds);
		in_writefds = *writefds;
		FD_ZERO(writefds);

		for (int libc_fd = 0; libc_fd < nfds; libc_fd++) {
			fdo = Libc::file_descriptor_allocator()->find_by_libc_fd(libc_fd);

			/* handle only libc_fds that belong to this plugin */
			if (!fdo || (fdo->plugin != this))
				continue;

			if (FD_ISSET(libc_fd, &in_readfds) &&
				is_read_end(fdo) &&
				!context(fdo)->buffer()->empty()) {
				FD_SET(libc_fd, readfds);
				nready++;
			}

			if (FD_ISSET(libc_fd, &in_writefds) &&
			    is_write_end(fdo) &&
			    (context(fdo)->buffer()->avail_capacity() > 0)) {
				FD_SET(libc_fd, writefds);
				nready++;
			}
		}
		return nready;
	}


	ssize_t Plugin::write(Libc::File_descriptor *fdo, const void *buf,
	                      ::size_t count)
	{
		if (!is_write_end(fdo)) {
			PERR("Cannot write into read end of pipe.");
			return -1;
		}

		/* TODO: thread-safety */
		/* TODO: non-blocking mode */

		for (::size_t num_bytes_written = 0;
		     num_bytes_written < count;
		     num_bytes_written++) {

			try {
				context(fdo)->buffer()->add(((unsigned char*)buf)[num_bytes_written]);
			} catch(Pipe_buffer::Overflow) {

				/* reset the 'write_avail_lock' to the 'locked' state */
				context(fdo)->write_avail_lock()->unlock();
				context(fdo)->write_avail_lock()->lock();

				/* notify the partner */
				if (libc_select_notify)
					libc_select_notify();

				/* wait until the partner has read from the buffer */
				context(fdo)->write_avail_lock()->lock();
			}
		}

		if (libc_select_notify)
			libc_select_notify();

		return count;
	}

}


void __attribute__((constructor)) init_libc_pipe()
{
	static Libc_pipe::Plugin plugin;
}
