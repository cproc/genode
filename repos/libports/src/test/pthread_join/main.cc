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
#include <stdlib.h>

void *thread2_run(void *arg)
{
	return arg;
}

void *thread1_run(void *arg)
{
	pthread_t  thread2;
	void      *expected_thread2_retval = (void*)2;
	void      *thread2_retval = 0;

	pthread_create(&thread2, 0, thread2_run, expected_thread2_retval);

	pthread_join(thread2, &thread2_retval);

	if (thread2_retval != expected_thread2_retval) {
		Genode::error("thread1_run(): return value does not match");
		exit(-1);
	}

	return arg;
}

int main()
{
	pthread_t  thread1;
	void      *expected_thread1_retval = (void*)1;
	void      *thread1_retval = 0;

	pthread_create(&thread1, 0, thread1_run, expected_thread1_retval);

	pthread_join(thread1, &thread1_retval);

	if (thread1_retval != expected_thread1_retval) {
		Genode::error("main(): return value does not match");
		return -1;
	}

	printf("--- returning from main ---\n");

	return 0;
}
