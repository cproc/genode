/*
 * \brief  POSIX thread and semaphore test
 * \author Christian Prochaska
 * \date   2012-04-04
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
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
	pthread_t p;

	pthread_create(&p, 0, run, 0);

	Genode::log("m: calling pthread_join()");

	pthread_join(p, 0);

	Genode::log("m: pthread_join() returned");

#if 0
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, 0);

	pthread_cond_t cond;
	pthread_cond_init(&cond, 0);

	Genode::log("calling pthread_cond_wait()");
	//pthread_cond_wait(&cond, &mutex);
	Genode::log("pthread_cond_wait() returned");
#endif

	return 0;
}
