#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread_func(void *)
{
	for (;;)
		sleep(3600);

	return 0;
}


int main()
{
	pthread_t t;
	
	getchar();
	
	pthread_create(&t, 0, thread_func, 0);
	
	for (;;)
		sleep(3600);
}
