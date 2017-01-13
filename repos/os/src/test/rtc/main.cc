/*
 * \brief  Test for RTC driver
 * \author Christian Helmuth
 * \date   2015-01-06
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/component.h>
#include <base/env.h>
#include <base/log.h>
#include <base/printf.h>
#include <rtc_session/connection.h>
#include <timer_session/connection.h>

struct Main
{
	Main(Genode::Env &env)
	{
		Genode::log("--- RTC test started ---");

		/* open sessions */
		Rtc::Connection   rtc(env);
		Timer::Connection timer(env);

		for (unsigned i = 0; i < 4; ++i) {
			Rtc::Timestamp now = rtc.current_time();

			Genode::printf("RTC: %u-%02u-%02u %02u:%02u:%02u\n",
		               	   now.year, now.month, now.day,
		               	   now.hour, now.minute, now.second);

			timer.msleep(1000);
		}

		Genode::log("--- RTC test finished ---");

		env.parent().exit(0);
	}
};

void Component::construct(Genode::Env &env) { static Main main(env); }
