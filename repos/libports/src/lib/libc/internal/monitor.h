/*
 * \brief  Monitored execution in main context
 * \author Christian Helmuth
 * \date   2020-01-09
 *
 * XXX may this replace the suspend mechanis/API ? For example
 *
 * read(...)
 *
 *   return Libc::monitor(
 *     [&] (Vfs &vfs) { step1 },
 *     [&] (Vfs &vfs) { step2 }, ...);
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

namespace Libc { class Monitor; };


struct Libc::Monitor : Interface
{
	struct Job;
	struct Pool;

	struct Function : Interface
	{
		/**
		 * Execute the monitored function
		 *
		 * The execution is retried until it succeeds.
		 */
		virtual bool execute() = 0;
	};

	template <typename FN>
	void monitor(FN const &&fn)
	{
		struct _Function : Function
		{
			FN const &fn;
			bool execute() override { return fn(); }
			_Function(FN const &fn) : fn(fn) { }
		} function { fn };

		monitor(function);
	}

	/**
	 * Blocks until monitored execution succeeds
	 */
	virtual void monitor(Function &) = 0;
//	virtual void monitor(Function &, Function &) = 0;

	virtual void charge_monitors() = 0;
};

struct Libc::Monitor::Job
{
	Monitor::Function &fn;

	Lock _blockade { Lock::LOCKED };

	Job(Monitor::Function &fn) : fn(fn) { }
	virtual ~Job() { }

	virtual void wait_for_completion() { _blockade.lock(); }
	virtual void complete()            { _blockade.unlock(); }
};

struct Libc::Monitor::Pool
{
	private:

		Registry<Job> _jobs;

		Lock _mutex;
		bool _execution_pending { false };

	public:

		/**
		 * Blocks until monitored execution succeeds
		 */
		void monitor(Job &job)
		{
			Registry<Job>::Element element { _jobs, job };

			_execution_pending = true;

			job.wait_for_completion();
		}

		bool charge_monitors()
		{
			Lock::Guard guard { _mutex };

			bool const charged = !_execution_pending;
			_execution_pending = true;
			return charged;
		}

		void execute_monitors()
		{
			{
				Lock::Guard guard { _mutex };

				//if (!_execution_pending) return;

				_execution_pending = false;
			}

			_jobs.for_each([&] (Job &job) {
				if (job.fn.execute()) {
					job.complete();
				}
			});
		}
};


#endif /* _LIBC__INTERNAL__MONITOR_H_ */
