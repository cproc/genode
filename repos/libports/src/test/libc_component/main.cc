/*
 * \brief  Libc-component using select test
 * \author Christian Helmuth
 * \date   2017-02-09
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/log.h>
#include <base/rpc_server.h>
#include <os/static_root.h>
#include <libc/component.h>
#include <libc/select.h>
#include <log_session/log_session.h>

/* libc includes */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/fcntl.h>


namespace Log {
	using Genode::Static_root;
	using Genode::Log_session;

	struct Session_component;
	struct Main;
}


static void die(char const *token) __attribute__((noreturn));
static void die(char const *token)
{
	Genode::error("[", Genode::Cstring(token), "] ", Genode::Cstring(strerror(errno)));
	exit(1);
}


static void use_file_system()
{
	int result = 0;

	int const fd = open("/tmp/blub", O_RDWR | O_NONBLOCK | O_CREAT | O_APPEND);
	if (fd == -1) die("open");

	printf("open returned fd %d\n", fd);

	static char buf[1024];

	result = read(fd, buf, sizeof(buf));
	if (result == -1) die("read");

	printf("read %d bytes\n", result);

	result = write(fd, "X", 1);
	if (result == -1) die("write");

	printf("wrote %d bytes\n", result);

	result = close(fd);
	if (result == -1) die("close");
}


struct Log::Session_component : Genode::Rpc_object<Log_session>
{
	char   _buf[Log_session::MAX_STRING_LEN];
	int    _fd = -1;
	fd_set _readfds;
	fd_set _dummyfds;
	bool   _select_scheduled = false;

	void _schedule_select()
	{
		_select_handler.schedule_select(_fd + 1, _readfds, _dummyfds, _dummyfds);
	}

	Libc::Select_handler<Session_component> _select_handler {
		*this, &Session_component::_on_select };

	void _on_select(int nready, fd_set const &readfds, fd_set const &, fd_set const &)
	{
		Libc::with_libc([&] () {
			if (nready <= 0) {
				Genode::warning("select handler reported nready=", nready);
				return;
			}
			if (!FD_ISSET(_fd, &readfds)) {
				Genode::warning("select handler reported unexpected fd");
				return;
			}
			int const result = read(_fd, _buf, sizeof(_buf)-1);
			if (result <= 0) {
				Genode::warning("read returned ", result, " in select handler");
				return;
			}
			_buf[result] = 0;
			Genode::log("read from file \"", Genode::Cstring(_buf), "\"");
		});

		_select_scheduled = false;
	}

	Session_component()
	{
		Libc::with_libc([&] () {
			_fd = open("/dev/terminal", O_RDWR);
			if (_fd == -1) die("open");

			FD_ZERO(&_readfds);
			FD_ZERO(&_dummyfds);
			FD_SET(_fd, &_readfds);
		});
	}

	size_t write(String const &string_buf)
	{
		if (!(string_buf.valid_string())) { return 0; }

		strncpy(_buf, string_buf.string(), sizeof(_buf));
		size_t len = strlen(_buf);

		if (_buf[len-1] == '\n') _buf[len-1] = 0;

		Genode::log("RPC with \"", Genode::Cstring(_buf), "\"");

		if (!_select_scheduled) {
			_schedule_select();
			_select_scheduled = true;
		}

		return len;
	}
};


struct Log::Main
{
	Libc::Env               &_env;
	Session_component        _session { };
	Static_root<Log_session> _root    { _env.ep().manage(_session) };

	Main(Libc::Env &env) : _env(env)
	{
		Libc::with_libc([] () { use_file_system(); });

		_env.parent().announce(_env.ep().manage(_root));
	}
};


void Libc::Component::construct(Libc::Env &env) { static Log::Main inst(env); }
