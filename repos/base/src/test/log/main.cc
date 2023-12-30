/*
 * \brief  Testing 'Genode::log()' and LOG session
 * \author Christian Prochaska
 * \date   2012-04-20
 *
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/component.h>
#include <base/log.h>
#include <log_session/connection.h>

using namespace Genode;

struct Test_thread : Thread
{
	Test_thread(Env &env) : Thread(env, "thread", 8192) { }

	void entry() override
	{
	}
};

void Component::construct(Genode::Env &env)
{
	for (;;) {
		Test_thread t(env);
	}
}
