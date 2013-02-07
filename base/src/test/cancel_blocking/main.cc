/* Genode includes */
#include <base/thread.h>
#include <base/printf.h>
#include <timer_session/connection.h>

enum { STACK_SIZE = 1024 * sizeof(Genode::addr_t) };

using namespace Genode;

static Lock thread_ready_lock(Lock::LOCKED);
static Lock test_lock(Lock::LOCKED);

struct Test_thread : Genode::Thread<STACK_SIZE>
{
	void entry()
	{
		thread_ready_lock.unlock();

		printf("thread: going to wait for the test lock\n");

		test_lock.lock();

		printf("thread: got the test lock\n");
	}
};


int main(int, char **)
{
	static Timer::Connection timer;

	static Test_thread thread;
	thread.start();

	thread_ready_lock.lock();

	printf("main: waiting 1s to make sure that the thread waits for the test lock'\n");

	/* make sure that the thread is waiting for the test lock */
	timer.msleep(1000);

	printf("main: calling 'cancel_blocking()'\n");

	thread.cancel_blocking();

	printf("main: waiting 1s to make sure that the thread waits for the test lock again'\n");

	/* make sure that the thread is waiting for the test lock again */
	timer.msleep(1000);

	printf("main: unlocking the test lock\n");

	test_lock.unlock();

	printf("main: unlocked the test lock\n");

	thread.join();

	return 0;
}
