/*
 * \brief  Lock implementation
 * \author Norman Feske
 * \date   2009-03-25
 */

/*
 * Copyright (C) 2009-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/cancelable_lock.h>

/* local includes */
#include "spin_lock.h"

using namespace Genode;

/**
 * Track interesting lock conditions, counters are only used for testing
 */
int debug_lock_sleep_race_cnt;


/********************
 ** Lock applicant **
 ********************/

void Cancelable_lock::Applicant::wake_up()
{
	if (!thread_id_valid(_tid)) {
		raw_write_str("Cancelable_lock::Applicant::wake_up(): invalid thread id\n");
		return;
	}

	/*
	 * Deal with the race that may occur in the 'lock' function between
	 * releasing the spinlock and calling 'L4_Stop'.
	 */

	for (;;) {

		if (thread_check_stopped_and_restart(_tid))
			return;

		debug_lock_sleep_race_cnt++;  /* only for statistics */
		thread_switch_to(_tid);
	}
}


/*********************
 ** Cancelable lock **
 *********************/
extern "C" void wait_for_continue();
void Cancelable_lock::lock()
{
	Applicant myself(thread_get_my_native_id());

	spinlock_lock(&_spinlock_state);

	//++_lock_count;

	/* reset ownership if one thread 'lock' twice */
	if (_owner == myself) {
		//_prev_owner = _owner;
		_owner = Applicant(thread_invalid_id());
	}

	if (cmpxchg(&_state, UNLOCKED, LOCKED)) {

		/* we got the lock */
		//_prev_owner     = _owner;
		_owner          =  myself;
		_last_applicant = &_owner;
		spinlock_unlock(&_spinlock_state);
		return;
	}

	/*
	 * We failed to grab the lock, lets add ourself to the
	 * list of applicants and block for the current lock holder.
	 */

	_last_applicant->applicant_to_wake_up(&myself);
	_last_applicant = &myself;
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
	thread_stop_myself();

	/*
	 * We expect to be the lock owner when woken up. If this is not
	 * the case, the blocking was canceled via core's cancel-blocking
	 * mechanism. We have to dequeue ourself from the list of applicants
	 * and reflect this condition as a C++ exception.
	 */
	spinlock_lock(&_spinlock_state);
	if (_owner != myself) {

		//PERR("_owner = %d, myself = %d, %d, %d, %d", _owner.tid().tid, myself.tid().tid, _lock_count, _unlock_count, *_owner.tid().uaddr2);
		PERR("_owner = %d, myself = %d, last FUTEX_WAIT result = %d", _owner.tid().tid, myself.tid().tid, *_owner.tid().uaddr2);
		wait_for_continue();
		/* check if we are the applicant to be waken up next */
		Applicant *a = _owner.applicant_to_wake_up();
		if (a && (*a == myself)) {

			_owner.applicant_to_wake_up(myself.applicant_to_wake_up());

		/* otherwise, go through the list of remaining applicants */
		} else {

			for (; a; a = a->applicant_to_wake_up()) {

				/* remove reference to ourself from the applicants list */
				if (a->applicant_to_wake_up() == &myself) {
					a->applicant_to_wake_up(myself.applicant_to_wake_up());
					break;
				}
			}
		}

		spinlock_unlock(&_spinlock_state);

		throw Blocking_canceled();
	}
	spinlock_unlock(&_spinlock_state);
}


void Cancelable_lock::unlock()
{
	spinlock_lock(&_spinlock_state);

	//++_unlock_count;

	Applicant *next_owner = _owner.applicant_to_wake_up();

	if (next_owner) {

		/* transfer lock ownership to next applicant and wake him up */
		//_prev_owner = _owner;
		_owner = *next_owner;
		if (_last_applicant == next_owner)
			_last_applicant = &_owner;

		//Applicant applicant_to_wake_up(_owner.tid());
		_owner.wake_up();
		spinlock_unlock(&_spinlock_state);

		//if (_owner != applicant_to_wake_up)
			//raw_write_str("owner changed!\n");

		//_owner.wake_up();

	} else {

		/* there is no further applicant, leave the lock alone */
		//_prev_owner     = _owner;
		_owner          = Applicant(thread_invalid_id());
		_last_applicant = 0;
		_state          = UNLOCKED;

		spinlock_unlock(&_spinlock_state);
	}
}


Cancelable_lock::Cancelable_lock(Cancelable_lock::State initial)
:
	//_check1(-12345),
	_spinlock_state(SPINLOCK_UNLOCKED),
	_state(UNLOCKED),
	_last_applicant(0),
	_owner(thread_invalid_id())
	//_check2(-67890),
	//_prev_owner(thread_invalid_id()),
	//_lock_count(0),
	//_unlock_count(0)
{
	if (initial == LOCKED)
		lock();
}

