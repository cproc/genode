#include <base/env.h>
#include <base/printf.h>
#include <base/thread.h>
#include <timer_session/connection.h>

using namespace Genode;


enum { STACK_SIZE = 1024*sizeof(addr_t) };


class Test_thread : public Genode::Thread<STACK_SIZE>
{
	private:

		Genode::Lock _ready_lock;
		volatile unsigned long _count;

	public:

		Test_thread() : Thread<STACK_SIZE>("test"),
		                _ready_lock(Lock::LOCKED),
		                _count(0) { }

		void entry()
		{
			_ready_lock.unlock();

			for(;;)
				_count++;
		}

		void wait_until_ready()
		{
			_ready_lock.lock();
		}

		unsigned long count()
		{
			return _count;
		}
};


int main(void)
{
	static Signal_context sig_ctx;
	static Signal_receiver sig_rec;
	static Signal_context_capability sig_cap = sig_rec.manage(&sig_ctx);

	static Timer::Connection timer;

	Test_thread test_thread;
	test_thread.start();
	test_thread.wait_until_ready();

	unsigned long count1;
	unsigned long count2;

	env()->cpu_session()->exception_handler(test_thread.cap(), sig_cap);

	while(1) {

		/* check if the running thread is making progress */

		count1 = test_thread.count();

		PDBG("count1 = %lu, waiting for progress...", count1);

		do {
			count2 = test_thread.count();
		} while (count2 == count1);

		PDBG("count2 = %lu", count2);


		PDBG("calling pause()");

		env()->cpu_session()->pause(test_thread.cap());;

		PDBG("pause() returned");

		/* check if the paused thread is making progress */

		count1 = test_thread.count();

		timer.msleep(10);

		count2 = test_thread.count();

		if (count2 != count1) {
			PERR("count 2 changed in paused state");
			for(;;);
		}

		PDBG("resuming thread in single-step mode");
		env()->cpu_session()->single_step(test_thread.cap(), true);
		env()->cpu_session()->resume(test_thread.cap());
		PDBG("resume() returned");

		timer.msleep(10);

		PDBG("calling pause()");

		env()->cpu_session()->pause(test_thread.cap());;

		PDBG("pause() returned");
		
		PDBG("resuming thread in normal mode");
		env()->cpu_session()->single_step(test_thread.cap(), false);
		env()->cpu_session()->resume(test_thread.cap());
		PDBG("resume() returned");
	}

	return 0;
}
