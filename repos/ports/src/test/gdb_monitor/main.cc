#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread_func(void *)
{
	for (;;) {
		fprintf(stderr, "pthread\n");
		sleep(1);
	}
}

int main()
{
	pthread_t t;
	pthread_create(&t, 0, thread_func, 0);

	for (;;) {
		fprintf(stderr, "main\n");
		sleep(1);
	}

	return 0;
}
