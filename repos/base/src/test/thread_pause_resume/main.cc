#include <base/env.h>
#include <base/printf.h>
#include <base/thread.h>

using namespace Genode;


enum { STACK_SIZE = 1024*sizeof(addr_t) };


class Test_thread : public Genode::Thread<STACK_SIZE>
{
	public:

		Test_thread() : Thread<STACK_SIZE>("test") { }

		void entry()
		{
			while(1);
		}
};


int main(void)
{
	while (1) {
		Test_thread test_thread;
		test_thread.start();

		PDBG("calling pause()");
		env()->cpu_session()->pause(test_thread.cap());
		PDBG("pause returned");
	}

	return 0;
}
