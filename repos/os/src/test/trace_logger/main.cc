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
#include <timer_session/connection.h>
#include <trace/timestamp.h>

using namespace Genode;


void Component::construct(Genode::Env &env)
{
	Timer::Connection timer(env);
	timer.msleep(1000);

	for (uint64_t i = 0x100000000; i <= 0x1000000cc; i++) {
		Genode::trace(Genode::Hex(i));
		if (i == 0x100000080)
			timer.msleep(2000);
	}

	timer.msleep(1000000);
	env.parent().exit(0);
}
