/*
 * \brief  Test for creating and destroying threads
 * \author Christian Prochaska
 * \date   2012-10-08
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>
#include <base/sleep.h>
#include <base/thread.h>

using namespace Genode;

enum { STACK_SIZE = 1024*sizeof(addr_t) };

struct Test_thread : Thread<STACK_SIZE>
{
	Lock ready_lock;

	Test_thread() : Thread<STACK_SIZE>(), ready_lock(Lock::LOCKED) { }

	void entry()
	{
		ready_lock.unlock();
	};

	void wait_until_ready()
	{
		ready_lock.lock();
	}
};

int main(int, char **)
{
	printf("--- thread test ---\n");

	for (int i = 0; i < 5000; i++) {
		printf("creating thread %d\n", i);
		Test_thread test_thread;
		test_thread.start();
		test_thread.wait_until_ready();
	}

	printf("--- thread test finished ---\n");
	return 0;
}
