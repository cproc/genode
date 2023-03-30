/*
 * \brief  Monitored execution in main context
 * \author Christian Helmuth
 * \author Christian Prochaska
 * \date   2020-01-09
 */

/*
 * Copyright (C) 2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _LIBC__INTERNAL__MONITOR_H_
#define _LIBC__INTERNAL__MONITOR_H_

/* Genode includes */
#include <base/registry.h>

/* libc-internal includes */
#include <internal/types.h>

#include <trace/probe.h>

namespace Libc { class Blockade; };


class Libc::Blockade
{
	protected:

		bool _woken_up { false };
		bool _expired  { false };

	public:

		bool woken_up() const { return _woken_up; }
		bool expired()  const { return _expired; }

		virtual void block(char const *name, char const *caller)  = 0;
		virtual void wakeup() = 0;
};


namespace Libc { class Monitor; };


class Libc::Monitor : Interface
{
	public:

		enum class Result { COMPLETE, TIMEOUT };

		struct Job;
		struct Pool;

		enum class Function_result { COMPLETE, INCOMPLETE };

		struct Function : Interface { virtual Function_result execute() = 0; };

	protected:

		virtual Result _monitor(Function &, uint64_t) = 0;
		virtual void _trigger_monitor_examination() = 0;

	public:

		/**
		 * Block until monitored execution completed or timeout expires
		 *
		 * Returns true if execution completed, false on timeout.
		 */
		template <typename FN>
		Result monitor(FN const &fn, uint64_t timeout_ms = 0)
		{
			struct _Function : Function
			{
				FN const &fn;
				Function_result execute() override { return fn(); }
				_Function(FN const &fn) : fn(fn) { }
			} function { fn };

			return _monitor(function, timeout_ms);
		}

		/**
		 * Trigger examination of monitored functions
		 */
		void trigger_monitor_examination() { _trigger_monitor_examination(); }
};


struct Libc::Monitor::Job
{
	private:

		Monitor::Function &_fn;
		Blockade          &_blockade;

	public:

		Job(Monitor::Function &fn, Blockade &blockade)
		: _fn(fn), _blockade(blockade) { }

		virtual ~Job() { }

		bool execute() { return _fn.execute() == Function_result::COMPLETE; }

		bool completed() const { return _blockade.woken_up(); }
		bool expired()   const { return _blockade.expired(); }

		void wait_for_completion() { _blockade.block("Monitor::Job", "Monitor::Job::wait_for_completion()"); }
		void complete()            { _blockade.wakeup(); }
};


struct Libc::Monitor::Pool
{
	private:

		Monitor       &_monitor;
		Registry<Job>  _jobs;

		struct Job_element {
			Genode::Mutex  mutex { };
			Job           *job   { nullptr };
		};

		Job_element _jobs_new[256] { };

	public:

		Pool(Monitor &monitor) : _monitor(monitor) { }

		/* called by monitor-user context */
		void monitor(Job &job)
		{
GENODE_TRACE_DURATION_NAMED(0, "monitor()");
			int dummy;
			unsigned char index = (((addr_t)&dummy) >> 20) & 0xff;
//Genode::log(index, ": monitor(): ", &job);

			{
				Genode::Mutex::Guard guard(_jobs_new[index].mutex, "monitor()");
				_jobs_new[index].job = &job;
			}

//			Registry<Job>::Element element { _jobs, job };
//Genode::log(index, ": monitor(): calling trigger_monitor_examination()");

			_monitor.trigger_monitor_examination();
//Genode::log(index, ": monitor(): trigger_monitor_examination() returned");

			job.wait_for_completion();

			if (!job.completed()) {
				Genode::Mutex::Guard guard(_jobs_new[index].mutex, "monitor()");
				_jobs_new[index].job = nullptr;
			}

//Genode::log(index, ": monitor() finished: ", job.completed());
		}

		enum class State { JOBS_PENDING, ALL_COMPLETE };

		/* called by the monitor context itself */
		State execute_monitors()
		{
//GENODE_TRACE_DURATION_NAMED(0, "execute_monitors()");

			State result = State::ALL_COMPLETE;

//			_jobs.for_each([&] (Job &job) {
//Genode::log("execute_monitors()");
			for (int i = 0; i < 256; i++) {

				Genode::Mutex::Guard(_jobs_new[i].mutex, "execute_monitors()");

				Job *job = _jobs_new[i].job;

//Genode::log("execute_monitors(): ", i);

				if (job && !job->completed() && !job->expired()) {
//Genode::log("execute_monitors(): found job: ", i);

					bool const completed = job->execute();

					if (completed) {
						_jobs_new[i].job = nullptr;
						job->complete();
					}

					if (!completed)
						result = State::JOBS_PENDING;
				}

			}
//			});
//Genode::log("execute_monitors() finished");

			return result;
		}
};

#endif /* _LIBC__INTERNAL__MONITOR_H_ */
