/*
 * \brief  Test program for suspending and resuming thread execution
 * \author Christian Prochaska
 * \date   2011-08-05
 */

/*
 * Copyright (C) 2011 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>
#include <base/sleep.h>
#include <base/thread.h>
#include <timer_session/connection.h>

using namespace Genode;


class Test_thread : public Genode::Thread<2*4096>
{
	private:

		addr_t set_registers()
		{
			/* set eax */
			return 0x12345678;
		}

	public:

		void entry()
		{
			//Lock lock(Lock::LOCKED);
			//lock.lock();
			//set_registers();
			while(1);
			//Genode::sleep_forever();
		}
};


void print_regs(Thread_state thread_state)
{
	PDBG(" ip = %8lx", thread_state.ip);
	PDBG(" sp = %8lx", thread_state.sp);
	PDBG("edi = %8lx", thread_state.edi);
	PDBG("esi = %8lx", thread_state.esi);
	PDBG("ebp = %8lx", thread_state.ebp);
	PDBG("ebx = %8lx", thread_state.ebx);
	PDBG("edx = %8lx", thread_state.edx);
	PDBG("ecx = %8lx", thread_state.ecx);
	PDBG("eax = %8lx", thread_state.eax);
	PDBG(" gs = %8lx", thread_state.gs);
	PDBG(" fs = %8lx", thread_state.fs);
	PDBG("efl = %8lx", thread_state.eflags);
}

int main(void)
{
	static Timer::Connection timer;

	Test_thread test_thread;
	test_thread.start();

	timer.msleep(1000);

	PDBG("calling pause()");
	env()->cpu_session()->pause(test_thread.cap());;
	PDBG("pause() returned");

	Thread_state thread_state = env()->cpu_session()->state(test_thread.cap());

	print_regs(thread_state);

	thread_state.edi = 0x11111111;
	thread_state.esi = 0x22222222;
	thread_state.ebp = 0x33333333;
	thread_state.ebx = 0x44444444;
	thread_state.edx = 0x55555555;
	thread_state.ecx = 0x66666666;
	thread_state.eax = 0x77777777;
	thread_state.sp  = 0x88888888;
	
	env()->cpu_session()->state(test_thread.cap(), thread_state);

	PDBG("calling resume()");
	env()->cpu_session()->resume(test_thread.cap());
	PDBG("resume() returned");

	timer.msleep(1000);

	PDBG("calling pause()");
	env()->cpu_session()->pause(test_thread.cap());;
	PDBG("pause() returned");

	thread_state = env()->cpu_session()->state(test_thread.cap());

	print_regs(thread_state);


	//PDBG("calling resume()");
	//env()->cpu_session()->resume(test_thread.cap());
	//PDBG("resume() returned");
while(1);
	return 0;
}
