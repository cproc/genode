/*
 * \brief  POSIX thread locking in combination with 'Libc::suspend()' test
 * \author Christian Prochaska
 * \date   2018-06-26
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <libc/component.h>

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *run(void *)
{
	Genode::log("t: run");

	int fd = open("/fs/zero", O_RDWR);
	Genode::log("t: fd: ", fd);

	for (;;) {
		char c;
		Genode::log("t: calling read()");
		read(fd, &c, sizeof(c));
		Genode::log("t: read() returned");
	}

	return 0;
}

int main()
{
	pthread_t thread;

	pthread_create(&thread, 0, run, 0);

	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, 0);

	pthread_cond_t cond;
	pthread_cond_init(&cond, 0);

	Genode::log("calling pthread_cond_wait()");
	pthread_cond_wait(&cond, &mutex);
	Genode::log("pthread_cond_wait() returned");

	pthread_join(thread, 0);

	return 0;
}
