/*
 * \brief  Testing thread creation and destruction
 * \author Christian Prochaska
 * \date   2018-06-25
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/log.h>
#include <base/thread.h>
#include <base/component.h>

using namespace Genode;

struct Test_thread : Thread
{
	enum { STACK_SIZE = 0x2000 };

	Test_thread(Env &env)
	:
		Thread(env, "test", STACK_SIZE, Thread::Location(), Thread::Weight(), env.cpu())
	{ }

	void entry() { }
};


void Component::construct(Env &env)
{
	log("--- thread creation and destruction test started ---");

	Constructible<Test_thread> thread;

	for (int i = 0; i < 1000; i++) {
		log("round ", i);
		thread.construct(env);
		thread->start();
		thread->join();
		thread.destruct();
	}

	log("--- test completed successfully ---");
}
