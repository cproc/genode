#include <base/env.h>
#include <base/printf.h>
#include <base/thread.h>

enum { STACK_SIZE = 4096 };

struct Test_thread : public Genode::Thread<STACK_SIZE>
{
	Test_thread() : Genode::Thread<STACK_SIZE>("test_thread") { }

	void entry()
	{
		PDBG("thread running, calling kill_thread() now");

		Genode::env()->cpu_session()->kill_thread(cap());
	}
};

int main(int argc, char **argv)
{
	static Test_thread test_thread;

	test_thread.start();
	
	for (;;)
		PDBG("main thread");

	return 0;
}
