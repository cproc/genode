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

class Test_thread : public Thread<STACK_SIZE>
{
	public:
		
		void func()
		{
			while(1);	
		}

		void entry()
		{
			wait_for_continue();
			func();
		}
};

int main(int argc, char **argv)
{
	static Test_thread test_thread;

	test_thread.start();

	sleep_forever();

	return 0;
}

