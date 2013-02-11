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


static int main_thread_futex_counter __attribute__((aligned(sizeof(Genode::addr_t)))) = 0;


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
#if 0
	/*
	 * Setting the futex counter to '1' here makes sure that the call of
	 * 'lx_futex(tid.uaddr, FUTEX_WAIT, 0)' in 'thread_stop_myself()'
	 * will return without blocking, because the kernel compares the
	 * current value at 'tid.uaddr' with the '0' given as argument and
	 * returns in case of mismatch.
	 *
	 * It must be made sure that the thread to be woken up cannot call
	 * 'lx_futex()' twice during the execution of the next two statements.
	 * Currently, this is ensured by protecting the '_owner.wake_up()'
	 * call in 'Cancelable_lock::unlock()' with a spinlock, which the
	 * thread to be woken up needs to get before it can call 'lx_futex()'
	 * for a second time.
	 */
	*tid.uaddr = 1;
#endif

	/* wake up at most 1 waiting thread */
	return lx_futex(tid.uaddr, FUTEX_WAKE, 1);
#if 0
	return true;
#endif
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
#if 0
	/* reset the futex counter */
	*tid.uaddr = 0;
#endif
}
