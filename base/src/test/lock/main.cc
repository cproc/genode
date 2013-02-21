/* Genode includes */
#include <base/thread.h>
#include <base/printf.h>

enum { STACK_SIZE = 1024 * sizeof(Genode::addr_t) };

using namespace Genode;

static Lock thread_lock;
static Lock main_lock;

struct Test_thread : Genode::Thread<STACK_SIZE>
{
	void entry()
	{
		unsigned long count = 0;

		for (;;) {
			thread_lock.lock();
			main_lock.unlock();
			if ((++count % 100000) == 0)
				PDBG("%lu: still running", count);
		}
	}
};


int main(int, char **)
{
	static Test_thread thread;
	thread.start();

	unsigned long count = 0;

	for (;;) {
		main_lock.lock();
		thread_lock.unlock();
		if ((++count % 100000) == 0)
			PDBG("%lu: still running", count);
	}

	return 0;
}
