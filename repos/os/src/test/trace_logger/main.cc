/*
 * \brief  Test functionality of the trace logger
 * \author Martin Stein
 * \date   2017-01-23
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <base/sleep.h>
#include <timer_session/connection.h>
#include <trace/timestamp.h>

using namespace Genode;


struct Test_thread : Genode::Thread
{
	Test_thread(Genode::Env &env) : Thread(env, "thread", 8192) { }

	void entry() override
	{
		for (unsigned i = 0; i <= 100; i++) {
			Thread::trace(String<32>("thread ", i, " ").string());
		}
	}
};

extern "C" void wait_for_continue();

void Component::construct(Genode::Env &env)
{
//wait_for_continue();

//	Thread::trace("sync main thread first");

//	static Timer::Connection timer(env);

	static Test_thread t(env);
	t.start();

Genode::log("Component::construct(): calling wait_and_dispatch_one_io_signal()");

	/* handle trace start signal of thread */
	env.ep().wait_and_dispatch_one_io_signal();

Genode::log("Component::construct(): wait_and_dispatch_one_io_signal() returned");

	for (unsigned i = 0; i <= 100; i++) {
//		Genode::log("main ", i);
		Thread::trace(String<32>("main ", i, " ").string());
//		timer.msleep(100);
	}

	Genode::sleep_forever();
	env.parent().exit(0);
}
