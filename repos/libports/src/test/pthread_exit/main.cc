#include <pthread.h>
#include <stdio.h>


void *thread_func(void *arg)
{
	return 0;
}


int main(int argc, char **argv)
{
	pthread_t thread;

	if (pthread_create(&thread, 0, thread_func, 0) != 0) {
		printf("error: pthread_create() failed\n");
		return -1;
	}

	while(1);

	return 0;
}
