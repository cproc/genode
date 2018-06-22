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
	return 0;
}

int main()
{
	pthread_t p;

	for (;;) {	
		pthread_create(&p, 0, run, 0);

		Genode::log("m: calling pthread_join()");

		pthread_join(p, 0);

		Genode::log("m: pthread_join() returned");
	}

	return 0;
}
