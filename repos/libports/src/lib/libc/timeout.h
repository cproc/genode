/*
 * \brief  Task timeouts
 * \author Christian Helmuth
 * \author Emery Hemingway
 * \date   2016-10-26
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _LIBC__TIMEOUT_H_
#define _LIBC__TIMEOUT_H_

/* Genode includes */
#include <base/log.h>
#include <os/alarm.h>
#include <timer_session/connection.h>

#include "task.h"

namespace Libc {

	Genode::Entrypoint & task_ep();

	struct Timer;
	struct Timeout;

}


class Libc::Timer : public Genode::Alarm_scheduler
{
	private:

		::Timer::Connection _timer;

		Genode::Signal_handler<Timer> _handler { task_ep(), *this, &Timer::_handle_timer };

		Genode::Alarm::Time _next_deadline = 0;

		void _trigger_next()
		{
			Genode::Alarm::Time next = 0;

			if (next_deadline(&next) && next != _next_deadline)
				_timer.trigger_once(next * 1000);

			_next_deadline = next;
		}

		void _handle_timer()
		{
			Genode::Alarm_scheduler::handle(_timer.elapsed_ms());

			task_resume();

			_trigger_next();
		}

	public:

		Timer() { _timer.sigh(_handler); }

		void schedule_timeout(Genode::Alarm *timeout, Genode::Alarm::Time duration_ms)
		{
			schedule_absolute(timeout, _timer.elapsed_ms() + duration_ms);

			_trigger_next();
		}

	static Timer & instance()
	{
		static Timer inst;
		return inst;
	}
};


struct Libc::Timeout : Genode::Alarm
{
	bool _triggered = false;

	Genode::Alarm::Time duration;

	Timeout(Genode::Alarm::Time duration)
	{
		Timer::instance().schedule_timeout(this, duration);
	}

	bool on_alarm(unsigned) override
	{
		_triggered = true;

		return false;
	}

	bool triggered() const { return _triggered; }
};

#endif /* _LIBC__TIMEOUT_H_ */