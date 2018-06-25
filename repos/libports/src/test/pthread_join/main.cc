/*
 * \brief  pthread_join() test
 * \author Christian Prochaska
 * \date   2018-06-25
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/log.h>

#include <pthread.h>
#include <stdio.h>

void *thread2_run(void *arg)
{
	//for (int i = 0; i < 5; i++)
		Genode::log(&arg, ": thread2_run()");
	return 0;
}

void *thread1_run(void *arg)
{
	Genode::log(&arg, ": thread1_run()");

	pthread_t thread2;

	Genode::log(&arg, ": calling pthread_create()");

	pthread_create(&thread2, 0, thread2_run, 0);

	Genode::log(&arg, ": calling pthread_join()");

	pthread_join(thread2, 0);

	Genode::log(&arg, ": pthread_join() returned");

	return 0;
}

int main()
{
	for (int i = 0; i < 1000; i++) {
		pthread_t thread1;

		Genode::log(&thread1, ": calling pthread_create(): ", i);

		pthread_create(&thread1, 0, thread1_run, 0);

		Genode::log(&thread1, ": calling pthread_join()");

		pthread_join(thread1, 0);

		Genode::log(&thread1, ": pthread_join() returned");
	}

	printf("--- returning from main ---\n");

	return 0;
}
