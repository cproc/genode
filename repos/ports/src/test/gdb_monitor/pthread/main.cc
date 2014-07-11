#include <pthread.h>

void step_func()
{
	while(1);
}

static void *thread_entry(void *arg)
{
	step_func();
}

void func()
{
	/* nothing */
}

int main(void)
{
	pthread_t pt;
	pthread_create(&pt, 0, thread_entry, 0);

	for(;;)
		func();

	return 0;
}
