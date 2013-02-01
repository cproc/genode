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
#include <base/snprintf.h>
#include <base/thread.h>

/* Linux includes */
#include <linux_syscalls.h>

#define VERBOSE

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
#ifdef VERBOSE
	char name[32] = "main";
	if (Genode::Thread_base::myself())
		Genode::Thread_base::myself()->name(name, sizeof (name) - 1);
	char buf[128];
#endif

#ifdef VERBOSE
	Genode::snprintf(buf, sizeof(buf), "%d (%s): sending signal to thread %d\n", lx_gettid(), name, tid.tid);
	raw_write_str(buf);
#endif

	lx_tgkill(tid.pid, tid.tid, LX_SIGUSR1);

#ifdef VERBOSE
	Genode::snprintf(buf, sizeof(buf), "%d (%s): sent signal to thread %d\n", lx_gettid(), name, tid.tid);
	raw_write_str(buf);
#endif

	return true;
}


static inline Genode::Native_thread_id thread_get_my_native_id()
{
	return Genode::Native_thread_id(lx_gettid(), lx_getpid());
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
#ifdef VERBOSE
	char name[32] = { 0 };
	if (Genode::Thread_base::myself())
		Genode::Thread_base::myself()->name(name, sizeof (name) - 1);
	char buf[128];
#endif

#ifdef VERBOSE
	Genode::snprintf(buf, sizeof(buf), "%d (%s): thread_stop_myself() called\n", lx_gettid(), name);
	raw_write_str(buf);
#endif

	struct timespec ts = { 1000, 0 };

#ifdef VERBOSE
	int ret = 0;
	while (ret == 0) {
		ret = lx_nanosleep(&ts, 0);
		Genode::snprintf(buf, sizeof(buf), "%d (%s): lx_nanosleep() returned %d\n", lx_gettid(), name, ret);
		raw_write_str(buf);
	}
#else
	while (lx_nanosleep(&ts, 0) == 0);
#endif

#ifdef VERBOSE
	Genode::snprintf(buf, sizeof(buf), "%d (%s): thread_stop_myself() returns\n", lx_gettid(), name);
	raw_write_str(buf);
#endif
}
