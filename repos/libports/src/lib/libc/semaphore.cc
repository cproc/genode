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

/* Genode includes */
#include <base/log.h>
#include <base/semaphore.h>
#include <semaphore.h>
#include <libc/allocator.h>

/* libc includes */
#include <errno.h>
#include <pthread.h> // XXX: only for debugging

/* libc-internal includes */
#include <internal/monitor.h>
#include <internal/types.h>

using namespace Libc;

extern Monitor *_monitor_ptr; /* XXX */

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
		Lock     _monitor_mutex;

		struct Missing_call_of_init_pthread_support : Exception { };

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

		sem(int value) : _count(value) { }

		int trydown()
		{
			Lock::Guard lock_guard(_data_mutex);
			
pthread_t const myself = pthread_self();

			if (_count > 0) {
				_count--;
Genode::log(this, ": trydown(): success, myself: ", myself, ", count: ", _count);
				return 0;
			}

Genode::log(this, ": trydown(): EBUSY, myself: ", myself, ", count: ", _count);
			return EBUSY;
		}

		int down()
		{
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
	
			return 0;
		}

		int up()
		{
			Lock::Guard monitor_guard(_monitor_mutex);

			Lock::Guard lock_guard(_data_mutex);

			_count++;

			if (!_monitor_ptr)
				throw Missing_call_of_init_pthread_support();

			pthread_t const myself = pthread_self();

Genode::log(this, ": up(): myself: ", myself, ", count: ", _count, ", applicants: ", _applicants);

			if (_applicants)
				_monitor_ptr->charge_monitors();

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
		return -1;
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
		return (*sem)->up();
	}


	int sem_timedwait(sem_t * __restrict, const struct timespec * __restrict)
	{
		warning(__func__, " not implemented");
		return -1;
	}


	int sem_trywait(sem_t *sem)
	{
		return (*sem)->trydown();
	}


	int sem_unlink(const char *)
	{
		warning(__func__, " not implemented");
		return -1;
	}


	int sem_wait(sem_t *sem)
	{
		return (*sem)->down();
	}

}
