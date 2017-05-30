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
#include <timer/timeout.h>
#include <trace/timestamp.h>

namespace Timer
{
	class Connection;
	template <typename> class Periodic_timeout;
	template <typename> class One_shot_timeout;
}


/**
 * Periodic timeout that is linked to a custom handler, scheduled when constructed
 */
template <typename HANDLER>
struct Timer::Periodic_timeout : private Genode::Noncopyable
{
	private:

		using Duration          = Genode::Duration;
		using Timeout           = Genode::Timeout;
		using Timeout_scheduler = Genode::Timeout_scheduler;
		using Microseconds      = Genode::Microseconds;

		typedef void (HANDLER::*Handler_method)(Duration);

		Timeout _timeout;

		struct Handler : Timeout::Handler
		{
			HANDLER              &object;
			Handler_method const  method;

			Handler(HANDLER &object, Handler_method method)
			: object(object), method(method) { }


			/**********************
			 ** Timeout::Handler **
			 **********************/

			void handle_timeout(Duration curr_time) override {
				(object.*method)(curr_time); }

		} _handler;

	public:

		Periodic_timeout(Timeout_scheduler &timeout_scheduler,
		                 HANDLER           &object,
		                 Handler_method     method,
		                 Microseconds       duration)
		:
			_timeout(timeout_scheduler), _handler(object, method)
		{
			_timeout.schedule_periodic(duration, _handler);
		}
};


/**
 * One-shot timeout that is linked to a custom handler, scheduled manually
 */
template <typename HANDLER>
class Timer::One_shot_timeout : private Genode::Noncopyable
{
	private:

		using Duration          = Genode::Duration;
		using Timeout           = Genode::Timeout;
		using Timeout_scheduler = Genode::Timeout_scheduler;
		using Microseconds      = Genode::Microseconds;

		typedef void (HANDLER::*Handler_method)(Duration);

		Timeout _timeout;

		struct Handler : Timeout::Handler
		{
			HANDLER              &object;
			Handler_method const  method;

			Handler(HANDLER &object, Handler_method method)
			: object(object), method(method) { }


			/**********************
			 ** Timeout::Handler **
			 **********************/

			void handle_timeout(Duration curr_time) override {
				(object.*method)(curr_time); }

		} _handler;

	public:

		One_shot_timeout(Timeout_scheduler &timeout_scheduler,
		                 HANDLER           &object,
		                 Handler_method     method)
		: _timeout(timeout_scheduler), _handler(object, method) { }

		void schedule(Microseconds duration) {
			_timeout.schedule_one_shot(duration, _handler); }

		void discard() { _timeout.discard(); }

		bool scheduled() { return _timeout.scheduled(); }
};


/**
 * Connection to timer service and timeout scheduler
 *
 * Multiplexes a timer session amongst different timeouts.
 */
class Timer::Connection : public  Genode::Connection<Session>,
                          public  Session_client,
                          private Genode::Time_source,
                          public  Genode::Timeout_scheduler
{
	private:

		using Timeout         = Genode::Timeout;
		using Timeout_handler = Genode::Time_source::Timeout_handler;
		using Timestamp       = Genode::Trace::Timestamp;
		using Duration        = Genode::Duration;
		using Lock            = Genode::Lock;
		using Microseconds    = Genode::Microseconds;
		using Milliseconds    = Genode::Milliseconds;
		using Entrypoint      = Genode::Entrypoint;

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


		/*************************
		 ** Time_source helpers **
		 *************************/

		/*
		 * The higher the factor shift, the more precise is the time
		 * interpolation but the more likely it becomes that an overflow
		 * would occur during calculations. In this case, the timer
		 * down-scales the values live which is avoidable overhead.
		 */
		enum { TS_TO_US_RATIO_SHIFT       = 4 };
		enum { MIN_TIMEOUT_US             = 5000 };
		enum { REAL_TIME_UPDATE_PERIOD_US = 500000 };
		enum { MAX_TS                     = ~(Timestamp)0ULL >> TS_TO_US_RATIO_SHIFT };
		enum { MAX_INTERPOLATION_QUALITY  = 3 };
		enum { MAX_REMOTE_TIME_LATENCY_US = 500 };
		enum { MAX_REMOTE_TIME_TRIALS     = 5 };

		Genode::Io_signal_handler<Connection> _signal_handler;

		Timeout_handler *_handler               { nullptr };
		Lock             _real_time_lock        { Lock::UNLOCKED };
		unsigned long    _ms                    { elapsed_ms() };
		Timestamp        _ts                    { _timestamp() };
		Duration         _real_time             { Milliseconds(_ms) };
		Duration         _interpolated_time     { _real_time };
		unsigned         _interpolation_quality { 0 };
		unsigned long    _us_to_ts_factor       { 1UL << TS_TO_US_RATIO_SHIFT };

		Timestamp _timestamp();

		void _update_interpolation_quality(unsigned long min_factor,
		                                   unsigned long max_factor);

		unsigned long _ts_to_us_ratio(Timestamp ts, unsigned long us);

		void _update_real_time();

		Duration _update_interpolated_time(Duration &interpolated_time);

		void _handle_timeout();


		/*****************
		 ** Time_source **
		 *****************/

		void schedule_timeout(Microseconds duration, Timeout_handler &handler) override;
		Microseconds max_timeout() const override { return Microseconds(REAL_TIME_UPDATE_PERIOD_US); }
		void scheduler(Timeout_scheduler &scheduler) override;


		/*******************************
		 ** Timeout_scheduler helpers **
		 *******************************/

		Genode::Alarm_timeout_scheduler _scheduler { *this };


		/***********************
		 ** Timeout_scheduler **
		 ***********************/

		void _schedule_one_shot(Timeout &timeout, Microseconds duration) override { _scheduler._schedule_one_shot(timeout, duration); };
		void _schedule_periodic(Timeout &timeout, Microseconds duration) override { _scheduler._schedule_periodic(timeout, duration); };
		void _discard(Timeout &timeout)                                  override { _scheduler._discard(timeout); }

	public:

		struct Can_not_use_blocking_interface_anymore : Genode::Exception { };

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
		 *
		 * \noapi
		 * \deprecated  Use One_shot_timeout (or Periodic_timeout) instead
		 */
		void sigh(Signal_context_capability sigh)
		{
			if (_scheduler._is_enabled()) {
				throw Can_not_use_blocking_interface_anymore();
			}
			_custom_sigh_cap = sigh;
			Session_client::sigh(_custom_sigh_cap);
		}

		/*
		 * Block for a time span of 'us' microseconds
		 *
		 * \noapi
		 * \deprecated  Use One_shot_timeout (or Periodic_timeout) instead
		 */
		void usleep(unsigned us)
		{
			if (_scheduler._is_enabled()) {
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

		/*
		 * Block for a time span of 'ms' milliseconds
		 *
		 * \noapi
		 * \deprecated  Use One_shot_timeout (or Periodic_timeout) instead
		 */
		void msleep(unsigned ms)
		{
			if (_scheduler._is_enabled()) {
				throw Can_not_use_blocking_interface_anymore();
			}
			usleep(1000*ms);
		}


		/***********************
		 ** Timeout_scheduler **
		 ***********************/

		Duration curr_time() override;
};

#endif /* _INCLUDE__TIMER_SESSION__CONNECTION_H_ */
