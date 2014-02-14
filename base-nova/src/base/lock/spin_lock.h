/*
 * \brief  Nova specific user land "Spin lock" implementation
 * \author Alexander Boettcher
 * \date   2014-02-07
 */

/*
 * Copyright (C) 2014-2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__LOCK__SPIN_H_
#define _INCLUDE__BASE__LOCK__SPIN_H_

/* Genode includes */
#include <cpu/atomic.h>
#include <base/thread.h>

/* local includes */
#include <lock_helper.h>


enum State {
	SPINLOCK_LOCKED = 0, SPINLOCK_UNLOCKED = 1, SPINLOCK_CONTENDED = 2,
};


template <typename T>
static inline void spinlock_lock(volatile T *lock_variable)
{
	using Genode::cmpxchg;

	Genode::Thread_base * myself = Genode::Thread_base::myself();
	T const tid = myself ? myself->tid().ec_sel : Nova::PT_SEL_MAIN_EC;

	unsigned help_counter = 0;

	if (myself) {
		Nova::Utcb * utcb  = reinterpret_cast<Nova::Utcb *>(myself->utcb());
		help_counter       = utcb->tls & 0xFFC;
	}

	/* try to get lock */
	do {
		T raw = *lock_variable;

		if (raw != SPINLOCK_UNLOCKED) {
			if (!(raw & SPINLOCK_CONTENDED))
				/* if it fails - just re-read and retry */
				if (!Genode::cmpxchg(lock_variable, raw, raw | SPINLOCK_CONTENDED))
					continue;

			/*
			 * Donate remaining time slice to help the spinlock holder to
			 * pass the critical section.
			 */
			unsigned long const ec  = raw >> 12;
			unsigned long const tls = raw & 0xFFC;
			Nova::ec_ctrl(Nova::EC_DONATE_SC, ec, tls);
			continue;
		}
	} while (!cmpxchg(lock_variable, (T)SPINLOCK_UNLOCKED,
	                  (tid << 12) | help_counter | SPINLOCK_LOCKED));
}


template <typename T>
static inline void spinlock_unlock(volatile T *lock_variable)
{
	using Nova::Utcb;

	Genode::Thread_base * myself = Genode::Thread_base::myself();
	Utcb * utcb = myself ? reinterpret_cast<Utcb *>(myself->utcb()) : 0;

	/* unlock */
	T old;
	do {
		old = *lock_variable;
	} while (!Genode::cmpxchg(lock_variable, old, (T)SPINLOCK_UNLOCKED));

	/* de-flag time donation help request and set new counter */
	if (utcb) {
		utcb->tls = (((utcb->tls & 0xFFC) + 4) % 4096) & 0xFFC;
		/* take care that compiler generates code that writes tls to memory */
		asm volatile ("":::"memory");
	}

	/*
	 * If anybody donated time, request kernel for a re-schedule in order that
	 * the helper can get its time donation (SC) back.
	 */
	if (old & SPINLOCK_CONTENDED)
		Nova::ec_ctrl(Nova::EC_RESCHEDULE);
}

#endif /* _INCLUDE__BASE__LOCK__SPIN_H_ */
