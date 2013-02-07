/* Genode includes */
#include <base/thread.h>
#include <base/printf.h>

enum { STACK_SIZE = 1024 * sizeof(Genode::addr_t) };

using namespace Genode;

static Semaphore thread_sem(1);
static Semaphore main_sem(1);

struct Test_thread : Genode::Thread<STACK_SIZE>
{
	void entry()
	{
		unsigned long long count = 0;

		for (;;) {
			thread_sem.down();
			main_sem.up();
			if ((++count % 100000) == 0)
				PDBG("%llu: still running", count);
		}
	}
};


int main(int, char **)
{
	static Test_thread thread;
	thread.start();

	unsigned long long count = 0;

	for (;;) {
		main_sem.down();
		thread_sem.up();
		if ((++count % 100000) == 0)
			PDBG("%llu: still running", count);
	}

	return 0;
}
