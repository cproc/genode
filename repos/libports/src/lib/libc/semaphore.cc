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
#include <pthread.h>

/* libc includes */
#include <libc/allocator.h>

/* libc-internal includes */
#include <internal/types.h>

using namespace Libc;


extern "C" {

	/*
	 * This class is named 'struct sem' because the 'sem_t' type is
	 * defined as 'struct sem*' in 'semaphore.h'
	 */
	struct sem : Genode::Semaphore
	{
		sem(int value) : Semaphore(value) { }
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
		*sval = (*sem)->cnt();
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
		(*sem)->up();
		return 0;
	}


	int sem_timedwait(sem_t * __restrict, const struct timespec * __restrict)
	{
		warning(__func__, " not implemented");
		return -1;
	}


	int sem_trywait(sem_t *)
	{
		warning(__func__, " not implemented");
		return -1;
	}


	int sem_unlink(const char *)
	{
		warning(__func__, " not implemented");
		return -1;
	}


	int sem_wait(sem_t *sem)
	{
		pthread_t const myself = pthread_self();

		Genode::log("sem_wait(): myself: ", myself);

		(*sem)->down();
		Genode::log("sem_wait() finished: myself: ", myself);

		return 0;
	}

}
