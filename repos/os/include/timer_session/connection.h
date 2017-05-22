/*
 * \brief  Connection to timer service and timeout scheduler
 * \author Norman Feske
 * \date   2008-08-22
 */

/*
 * Copyright (C) 2008-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__TIMER_SESSION__CONNECTION_H_
#define _INCLUDE__TIMER_SESSION__CONNECTION_H_

/* Genode includes */
#include <timer_session/client.h>
#include <base/connection.h>
#include <util/reconstructible.h>
#include <base/entrypoint.h>
#include <os/time_source.h>
#include <os/timeout.h>
#include <trace/timestamp.h>

namespace Genode { class Timer_time_source; }

namespace Timer { class Connection; }

/**
 * Implementation helper for 'Timer'
 *
 * \noapi
 */
class Genode::Timer_time_source : public Genode::Time_source
{
	private:

		/*
		 * The higher the factor shift, the more precise is the time
		 * interpolation but the more likely it becomes that an overflow
		 * would occur during calculations. In this case, the timer
		 * down-scales the values live which is avoidable overhead.
		 */
		enum { TS_TO_US_RATIO_SHIFT       = 8 };
		enum { MIN_TIMEOUT_US             = 5000 };
		enum { REAL_TIME_UPDATE_PERIOD_US = 100000 };
		enum { MAX_TS                     = ~(Trace::Timestamp)0ULL >> TS_TO_US_RATIO_SHIFT };
		enum { MAX_INTERPOLATION_QUALITY  = 3 };
		enum { MAX_REMOTE_TIME_LATENCY_US = 500 };
		enum { MAX_REMOTE_TIME_TRIALS     = 5 };

		::Timer::Connection                                 &_connection;
		Io_signal_handler<Timer_time_source>                 _signal_handler;
		Timeout_handler                                     *_handler = nullptr;
		Constructible<Periodic_timeout<Timer_time_source> >  _real_time_update;

		Lock             _real_time_lock        { Lock::UNLOCKED };
		unsigned long    _ms;
		Trace::Timestamp _ts                    { _timestamp() };
		Duration         _real_time             { Milliseconds(_ms) };
		Duration         _interpolated_time     { _real_time };
		unsigned         _interpolation_quality { 0 };
		unsigned long    _us_to_ts_factor       { 1UL << TS_TO_US_RATIO_SHIFT };

		Trace::Timestamp _timestamp();

		void _update_interpolation_quality(unsigned long min_factor,
		                                   unsigned long max_factor);

		unsigned long _ts_to_us_ratio(Trace::Timestamp ts, unsigned long us);

		void _handle_real_time_update(Duration);

		Duration _update_interpolated_time(Duration &interpolated_time);

		void _handle_timeout();

	public:

		Timer_time_source(Entrypoint          &ep,
		                  ::Timer::Connection &session);


		/*****************
		 ** Time_source **
		 *****************/

		void schedule_timeout(Microseconds     duration,
		                      Timeout_handler &handler) override;

		Microseconds max_timeout() const override { return Microseconds::max(); }

		Duration curr_time() override;

		void scheduler(Timeout_scheduler &scheduler) override;
};


/**
 * Connection to timer service and timeout scheduler
 *
 * Multiplexes a timer session amongst different timeouts.
 */
class Timer::Connection : public Genode::Connection<Session>,
                          public Session_client,
                          public Genode::Timer_time_source,
                          public Genode::Alarm_timeout_scheduler
{
	friend class Genode::Timer_time_source;

	private:

		Genode::Lock            _lock;
		Genode::Signal_receiver _sig_rec;
		Genode::Signal_context  _default_sigh_ctx;

		Genode::Signal_context_capability
			_default_sigh_cap = _sig_rec.manage(&_default_sigh_ctx);

		Genode::Signal_context_capability _custom_sigh_cap;

		void _sigh(Signal_context_capability sigh)
		{
			Session_client::sigh(sigh);
		}

	public:

		struct Can_not_use_blocking_interface_anymore : Genode::Exception { };

		using Alarm_timeout_scheduler::curr_time;

		/**
		 * Constructor
		 */
		Connection(Genode::Env &env, char const *label = "");

		/**
		 * Constructor
		 *
		 * \noapi
		 * \deprecated  Use the constructor with 'Env &' as first
		 *              argument instead
		 */
		Connection() __attribute__((deprecated));

		~Connection() { _sig_rec.dissolve(&_default_sigh_ctx); }

		/*
		 * Intercept 'sigh' to keep track of customized signal handlers
		 */
		void sigh(Signal_context_capability sigh)
		{
			if (Alarm_timeout_scheduler::_is_enabled()) {
				throw Can_not_use_blocking_interface_anymore();
			}
			_custom_sigh_cap = sigh;
			Session_client::sigh(_custom_sigh_cap);
		}

		void usleep(unsigned us)
		{
			if (Alarm_timeout_scheduler::_is_enabled()) {
				throw Can_not_use_blocking_interface_anymore();
			}
			/*
			 * Omit the interaction with the timer driver for the corner case
			 * of not sleeping at all. This corner case may be triggered when
			 * polling is combined with sleeping (as some device drivers do).
			 * If we passed the sleep operation to the timer driver, the
			 * timer would apply its policy about a minimum sleep time to
			 * the sleep operation, which is not desired when polling.
			 */
			if (us == 0)
				return;

			/* serialize sleep calls issued by different threads */
			Genode::Lock::Guard guard(_lock);

			/* temporarily install to the default signal handler */
			if (_custom_sigh_cap.valid())
				Session_client::sigh(_default_sigh_cap);

			/* trigger timeout at default signal handler */
			trigger_once(us);
			_sig_rec.wait_for_signal();

			/* revert custom signal handler if registered */
			if (_custom_sigh_cap.valid())
				Session_client::sigh(_custom_sigh_cap);
		}

		void msleep(unsigned ms)
		{
			if (Alarm_timeout_scheduler::_is_enabled()) {
				throw Can_not_use_blocking_interface_anymore();
			}
			usleep(1000*ms);
		}
};

#endif /* _INCLUDE__TIMER_SESSION__CONNECTION_H_ */
