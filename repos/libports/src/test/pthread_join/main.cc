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

#include <pthread.h>
#include <stdio.h>

void *thread2_run(void *)
{
	return 0;
}

void *thread1_run(void *)
{
	pthread_t thread2;
	pthread_create(&thread2, 0, thread2_run, 0);
	pthread_join(thread2, 0);
	
	return 0;
}

int main()
{
	pthread_t thread1;
	pthread_create(&thread1, 0, thread1_run, 0);
	pthread_join(thread1, 0);

	printf("--- returning from main ---\n");

	return 0;
}
