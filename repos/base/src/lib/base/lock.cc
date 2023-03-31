/*
 * \brief  Lock implementation
 * \author Norman Feske
 * \date   2009-03-25
 */

/*
 * Copyright (C) 2009-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/lock.h>
#include <cpu/memory_barrier.h>

/* base-internal includes */
#include <base/internal/spin_lock.h>

#include <trace/probe.h>

using namespace Genode;


static inline Genode::Thread *invalid_thread_base()
{
	return (Genode::Thread*)~0UL;
}


static inline bool thread_base_valid(Genode::Thread *thread_base)
{
	return (thread_base != invalid_thread_base());
}


/********************
 ** Lock applicant **
 ********************/

void Lock::Applicant::wake_up()
{
	if (!thread_base_valid(_thread_base)) return;

	/*
	 * Deal with the race that may occur in the 'lock' function between
	 * releasing the spinlock and blocking myself.
	 */

	for (;;) {

		if (thread_check_stopped_and_restart(_thread_base))
			return;

		thread_switch_to(_thread_base);
	}
}


/***************
 ** Lock lock **
 ***************/

void Lock::lock(char const *name, char const *caller)
{
	Applicant myself(Thread::myself());
	lock(myself, name, caller);
}

void Lock::lock(Applicant &myself, char const *name, char const *caller)
{
	spinlock_lock(&_spinlock_state);

	if (cmpxchg(&_state, UNLOCKED, LOCKED)) {

		/* we got the lock */
		_owner          =  myself;
		_last_applicant = &_owner;
//Genode::Trace::Lock_locked(this);
		spinlock_unlock(&_spinlock_state);
		return;
	}

if (caller) {
Trace::Checkpoint(caller, 0, nullptr, Trace::Checkpoint::Type::START);
}

//Genode::Trace::Lock_wait(this);
if (_owner.thread_base()) {
Trace::Lock_wait(this, _owner.thread_base()->name().string(), name);
}

	/*
	 * We failed to grab the lock, lets add ourself to the
	 * list of applicants and block for the current lock holder.
	 */

	/* reset ownership if one thread 'lock' twice */
	if (_owner == myself) {
		/* remember applicants already in list */
		Applicant * applicants =_owner.applicant_to_wake_up();

		/* reset owner */
		_owner = Applicant(invalid_thread_base());

		/* register thread calling twice 'lock' as first applicant */
		_owner.applicant_to_wake_up(&myself);

		/* if we had already applicants, add after myself in list */
		myself.applicant_to_wake_up(applicants);

		/* if we had applicants, _last_applicant already points to the last */
		if (!applicants)
			_last_applicant = &myself;
	} else {
//int dummy;
//Genode::raw(&dummy, ": _owner: ", _owner.thread_base()->stack_base());
//Genode::raw(&dummy, ": _owner: ", Genode::Cstring(_owner.thread_base()->name().string()));
		if (_last_applicant)
			_last_applicant->applicant_to_wake_up(&myself);
		_last_applicant = &myself;
	}

	spinlock_unlock(&_spinlock_state);

	/*
	 * At this point, a race can happen. We have added ourself to the wait
	 * queue but do not block yet. If we get preempted here, the lock holder
	 * may call 'unlock' and thereby find us as the next applicant to wake up.
	 * However, the 'L4_Start' call will then be issued before we went to sleep
	 * via 'L4_Stop'. When we get scheduled for the next time, we are expected
	 * to enter the critical section but we will execute 'L4_Stop' instead.
	 * We handle this case in the 'unlock' function by checking the previous
	 * thread state when resuming its execution.
	 *
	 * Note for testing: To artificially increase the chance for triggering the
	 * race condition, we can delay the execution here. For example via:
	 *
	 * ! for (int i = 0; i < 10; i++)
	 * !   thread_yield();
	 */
	thread_stop_myself(myself.thread_base());

Genode::Trace::Lock_locked(this);

if (caller) {
Trace::Checkpoint(caller, 0, nullptr, Trace::Checkpoint::Type::END);
}

}


void Lock::unlock()
{
	spinlock_lock(&_spinlock_state);

//Genode::Trace::Lock_unlock(this);

	Applicant *next_owner = _owner.applicant_to_wake_up();

	if (next_owner) {

if (next_owner->thread_base()) {
Trace::Lock_unlock(this, next_owner->thread_base()->name().string());
}
		/* transfer lock ownership to next applicant and wake him up */
		_owner = *next_owner;

		/* make copy since _owner may change outside spinlock ! */
		Applicant owner = *next_owner;

		if (_last_applicant == next_owner)
			_last_applicant = &_owner;

		spinlock_unlock(&_spinlock_state);

		owner.wake_up();

	} else {

		/* there is no further applicant, leave the lock alone */
		_owner          = Applicant(invalid_thread_base());
		_last_applicant = 0;
		_state          = UNLOCKED;

		spinlock_unlock(&_spinlock_state);
	}
}


Lock::Lock(Lock::State initial)
:
	_spinlock_state(SPINLOCK_UNLOCKED),
	_state(UNLOCKED),
	_last_applicant(0),
	_owner(invalid_thread_base())
{
	if (initial == LOCKED)
		lock();
}
