/*
 * \brief  Test for writing via VFS from a pthread
 * \author Christian Prochaska
 * \date   2017-06-02
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/log.h>
#include <libc/component.h>

/* libc includes */
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
extern "C" void wait_for_continue();
void *write_func(void*)
{
	Genode::log("write_func()");
//wait_for_continue();
	char buf[1024];

	for (;;) {

		int fd = open("test", O_CREAT | O_RDWR);
		Genode::log("fd: ", fd);

		unsigned int bytes_written = 0;

		for (int i = 0; i < 124; i++) {
			ssize_t written = write(fd, buf, sizeof(buf));
			bytes_written += written;
			Genode::log("bytes written: ", written, " (", bytes_written, ")");
		}

		Genode::log("closing file");
		close(fd);
	}
}

struct Main
{
	Main()
	{
		pthread_t thread;
		pthread_create(&thread, 0, write_func, 0);
	}
};

void Libc::Component::construct(Libc::Env &)
{
	static Main main;
}
