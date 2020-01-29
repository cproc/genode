/*
 * \brief  POSIX semaphore implementation
 * \author Christian Prochaska
 * \date   2012-03-12
 *
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#define USE_MONITOR 0

/* Genode includes */
#include <base/log.h>
#include <base/semaphore.h>
#include <semaphore.h>
#include <libc/allocator.h>

/* libc includes */
#include <errno.h>
#include <time.h>

/* libc-internal includes */
#if USE_MONITOR
#include <internal/monitor.h>
#else
#include <internal/suspend.h>
#include <internal/resume.h>
#endif
#include <internal/errno.h>
#include <internal/types.h>

using namespace Libc;


#if USE_MONITOR
extern Monitor *_monitor_ptr;
#else
extern Suspend *_suspend_ptr;
extern Resume  *_resume_ptr;
#endif


extern "C" {

	/*
	 * This class is named 'struct sem' because the 'sem_t' type is
	 * defined as 'struct sem*' in 'semaphore.h'
	 */
	struct sem
	{
		int      _count;
		unsigned _applicants { 0 };
		Lock     _data_mutex;
#if USE_MONITOR
		Lock     _monitor_mutex;
#endif

		struct Missing_call_of_init_pthread_support : Exception { };

#if USE_MONITOR
		struct Applicant
		{
			sem &s;

			Applicant(sem &s) : s(s)
			{
				Lock::Guard lock_guard(s._data_mutex);
				++s._applicants;
			}

			~Applicant()
			{
				Lock::Guard lock_guard(s._data_mutex);
				--s._applicants;
			}
		};
#else
		uint64_t _suspend(Suspend_functor &func, uint64_t relative_timeout_ms = 0)
		{
			if (!_suspend_ptr)
				throw Missing_call_of_init_pthread_support();
			return _suspend_ptr->suspend(func, relative_timeout_ms);
		}

		void _resume_all()
		{
			if (!_resume_ptr)
				throw Missing_call_of_init_pthread_support();
			_resume_ptr->resume_all();
		}
#endif

		sem(int value) : _count(value) { }

		int trydown()
		{
			Lock::Guard lock_guard(_data_mutex);
			
			if (_count > 0) {
				_count--;
				return 0;
			}

			return EBUSY;
		}

		int down()
		{
#if USE_MONITOR
			Lock::Guard lock_guard(_monitor_mutex);

			/* fast path without contention */
			if (trydown() == 0)
				return 0;

			{
				Applicant guard { *this };

				if (!_monitor_ptr)
					throw Missing_call_of_init_pthread_support();

				_monitor_ptr->monitor(_monitor_mutex,
				                      [&] { return trydown() == 0; });
			}
#else
			struct Try_down : Suspend_functor
			{
				bool retry { false }; /* have to try after resume */

				sem &_sem;

				Try_down(sem &sem) : _sem(sem) { }

				bool suspend() override
				{
					retry = _sem.trydown() == EBUSY;
					return retry;
				}
			} try_down(*this);

			do { _suspend(try_down); } while (try_down.retry);
#endif

			return 0;
		}

		int down_timed(uint64_t relative_timeout_ms)
		{
#if USE_MONITOR
#error implementation missing
#else
			struct Try_down : Suspend_functor
			{
				bool retry { false }; /* have to try after resume */

				sem &_sem;

				Try_down(sem &sem) : _sem(sem) { }

				bool suspend() override
				{
					retry = _sem.trydown() == EBUSY;
					return retry;
				}
			} try_down(*this);

			do {
				relative_timeout_ms = _suspend(try_down, relative_timeout_ms);
			} while (try_down.retry && relative_timeout_ms);

			if (!try_down.retry)
				return 0;

			return ETIMEDOUT;
#endif
		}

		int up()
		{
#if USE_MONITOR
			Lock::Guard monitor_guard(_monitor_mutex);
#endif
			Lock::Guard lock_guard(_data_mutex);

			_count++;

#if USE_MONITOR
			if (!_monitor_ptr)
				throw Missing_call_of_init_pthread_support();

			if (_applicants)
				_monitor_ptr->charge_monitors();
#else
			_resume_all();
#endif

			return 0;
		}

		int count()
		{
			return _count;
		}
	};


	int sem_close(sem_t *)
	{
		warning(__func__, " not implemented");
		return Errno(ENOSYS);
	}


	int sem_destroy(sem_t *sem)
	{
		Libc::Allocator alloc { };
		destroy(alloc, *sem);
		return 0;
	}


	int sem_getvalue(sem_t * __restrict sem, int * __restrict sval)
	{
		*sval = (*sem)->count();
		return 0;
	}


	int sem_init(sem_t *sem, int pshared, unsigned int value)
	{
		Libc::Allocator alloc { };
		*sem = new (alloc) struct sem(value);
		return 0;
	}


	sem_t *sem_open(const char *, int, ...)
	{
		warning(__func__, " not implemented");
		return 0;
	}


	int sem_post(sem_t *sem)
	{
		if (int res = (*sem)->up())
			return Errno(res);

		return 0;
	}


	static uint64_t calculate_relative_timeout_ms(struct timespec currtime,
	                                    struct timespec abstimeout)
	{
		enum { S_IN_MS = 1000, S_IN_NS = 1000 * 1000 * 1000 };

		if (currtime.tv_nsec >= S_IN_NS) {
			currtime.tv_sec  += currtime.tv_nsec / S_IN_NS;
			currtime.tv_nsec  = currtime.tv_nsec % S_IN_NS;
		}
		if (abstimeout.tv_nsec >= S_IN_NS) {
			abstimeout.tv_sec  += abstimeout.tv_nsec / S_IN_NS;
			abstimeout.tv_nsec  = abstimeout.tv_nsec % S_IN_NS;
		}

		/* check whether absolute timeout is in the past */
		if (currtime.tv_sec > abstimeout.tv_sec)
			return 0;

		uint64_t diff_ms = (abstimeout.tv_sec - currtime.tv_sec) * S_IN_MS;
		uint64_t diff_ns = 0;

		if (abstimeout.tv_nsec >= currtime.tv_nsec)
			diff_ns = abstimeout.tv_nsec - currtime.tv_nsec;
		else {
			/* check whether absolute timeout is in the past */
			if (diff_ms == 0)
				return 0;
			diff_ns  = S_IN_NS - currtime.tv_nsec + abstimeout.tv_nsec;
			diff_ms -= S_IN_MS;
		}

		diff_ms += diff_ns / 1000 / 1000;

		/* if there is any diff then let the timeout be at least 1 MS */
		if (diff_ms == 0 && diff_ns != 0)
			return 1;

		return diff_ms;
	}


	int sem_timedwait(sem_t * __restrict sem, const struct timespec * __restrict abstime)
	{
		struct timespec currtime;
		clock_gettime(CLOCK_REALTIME, &currtime);

		uint64_t relative_timeout_ms = calculate_relative_timeout_ms(currtime, *abstime);

		if (int res = (*sem)->down_timed(relative_timeout_ms))
			return Errno(res);

		return 0;
	}


	int sem_trywait(sem_t *sem)
	{
		if (int res = (*sem)->trydown())
			return Errno(res);

		return 0;
	}


	int sem_unlink(const char *)
	{
		warning(__func__, " not implemented");
		return Errno(ENOSYS);
	}


	int sem_wait(sem_t *sem)
	{
		if (int res = (*sem)->down())
			return Errno(res);

		return 0;
	}

}
