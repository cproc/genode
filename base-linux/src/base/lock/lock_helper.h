/*
 * \brief  Linux-specific helper functions for the Lock implementation
 * \author Norman Feske
 * \date   2009-07-20
 *
 * This file serves as adapter between the generic lock implementation
 * in 'lock.cc' and the underlying kernel.
 *
 * For documentation about the interface, please revisit the 'base-pistachio'
 * implementation.
 */

/*
 * Copyright (C) 2009-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/native_types.h>
#include <base/thread.h>

/* Linux includes */
#include <linux_syscalls.h>


static int main_thread_futex_counter = 0;


/**
 * Resolve 'Thread_base::myself' when not linking the thread library
 *
 * This weak symbol is primarily used by test cases. Most other Genode programs
 * use the thread library. If the thread library is not used, 'myself' can only
 * be called by the main thread, for which 'myself' is defined as zero.
 */
Genode::Thread_base * __attribute__((weak)) Genode::Thread_base::myself() { return 0; }


static inline void thread_yield()
{
	struct timespec ts = { 0, 1000 };
	lx_nanosleep(&ts, 0);
}


static inline bool thread_check_stopped_and_restart(Genode::Native_thread_id tid)
{
	/*
	 * Setting the futex counter to '1' here makes sure that the call of
	 * 'lx_futex(tid.uaddr, FUTEX_WAIT, 0)' in 'thread_stop_myself()'
	 * will return without blocking, because the kernel compares the
	 * current value at 'tid.uaddr' with the '0' given as argument and
	 * returns in case of mismatch.
	 */
	*tid.uaddr = 1;

	/* wake up at most 1 waiting thread */
	lx_futex(tid.uaddr, FUTEX_WAKE, 1);

	return true;
}


static inline Genode::Native_thread_id thread_get_my_native_id()
{
	Genode::Thread_base *myself = Genode::Thread_base::myself();
	return myself
	       ? myself->tid()
	       : Genode::Native_thread_id(lx_gettid(), lx_getpid(),
	                                  &main_thread_futex_counter);
}


static inline Genode::Native_thread_id thread_invalid_id()
{
	return Genode::Native_thread_id();
}


static inline bool thread_id_valid(Genode::Native_thread_id tid)
{
	return (tid.pid != 0);
}


static inline void thread_switch_to(Genode::Native_thread_id tid)
{
	thread_yield();
}


static inline void thread_stop_myself()
{
	Genode::Native_thread_id tid = thread_get_my_native_id();

	/*
	 * This call returns if the current value of 'tid.uaddr' is not '0'
	 * anymore or when 'lx_futex(tid.uaddr, FUTEX_WAKE, 1)' gets called
	 * in 'thread_check_stopped_and_restart()'.
	 */
	lx_futex(tid.uaddr, FUTEX_WAIT, 0);

	/* reset the futex counter */
	*tid.uaddr = 0;
}
