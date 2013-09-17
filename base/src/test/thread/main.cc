/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>
#include <base/sleep.h>
#include <base/thread.h>
 
using namespace Genode;

extern "C" void wait_for_continue();

enum { STACK_SIZE = 2*1024*sizeof(addr_t) };

static bool trigger = false;

void func()
{
	static volatile int c = 0;
	c++;
}

class Test_thread : public Thread<STACK_SIZE>
{
	public:
		
		void entry()
		{
			while (!trigger)
				func();
		}
};

class Test_thread2 : public Thread<STACK_SIZE>
{
	public:
		
		void entry()
		{
			wait_for_continue();
			trigger = true;
			for (;;)
				func();
		}
};


int main(int argc, char **argv)
{
	static Test_thread test_thread;
	static Test_thread2 test_thread2;

	test_thread.start();
	test_thread2.start();

#if 0
	wait_for_continue();
	trigger = true;	
	for (;;)
		func();
#endif

	sleep_forever();

	PDBG("main thread returned from sleep_forever()");

	return 0;
}

