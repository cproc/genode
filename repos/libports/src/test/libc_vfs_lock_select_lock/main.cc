/*
 * \brief  libc_vfs lock / select callback list lock test
 * \author Christian Prochaska
 * \date   2018-06-29
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/log.h>

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static int fd;

void *thread2_func(void *)
{
	for (;;) {
		char c;
		Genode::log("t2: calling read()");
		read(fd, &c, sizeof(c));
		Genode::log("t2: read() returned");
	}

	return 0;
}

void *thread1_func(void *)
{
	fd_set readfds;
	struct timeval timeout { 0, 0 };

	for (;;) {
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		Genode::log("t1: calling select()");
		select(fd + 1, &readfds, 0, 0, &timeout);
		Genode::log("t1: select() returned");
	}
}

int main()
{
	fd = open("/fs/zero", O_RDWR);
	Genode::log("fd: ", fd);

	pthread_t thread1;
	pthread_t thread2;

	pthread_create(&thread1, 0, thread1_func, 0);
	pthread_create(&thread2, 0, thread2_func, 0);

	pthread_join(thread1, 0);
	pthread_join(thread2, 0);

	return 0;
}
