/*
 * \brief  Helper functions for the lock implementation
 * \author Martin Stein
 * \date   2011-01-02
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__BASE__INTERNAL__LOCK_HELPER_H_
#define _INCLUDE__BASE__INTERNAL__LOCK_HELPER_H_

#include <trace/probe.h>

/* Genode includes */
#include <base/thread.h>

/* base-internal includes */
#include <base/internal/capability_space.h>

namespace Hw { extern Genode::Untyped_capability _main_thread_cap; }


/**
 * Yield execution time-slice of current thread
 */
static inline void thread_yield() { Kernel::yield_thread(); }


/**
 * Return kernel name of thread t
 */
static inline Kernel::capid_t
native_thread_id(Genode::Thread * const t)
{
	using Genode::Capability_space::capid;
	return t ? capid(t->native_thread().cap) : capid(Hw::_main_thread_cap);
}


/**
 * Yield execution time-slice of current thread to thread t
 */
static inline void thread_switch_to(Genode::Thread *)
{
	Kernel::yield_thread();
}


/**
 * Resume thread t and return wether t was paused or not
 */
static inline bool
thread_check_stopped_and_restart(Genode::Thread * const t)
{
#if 0
for (int i = 0; i < 5; i++) {
Genode::raw(i, ": ", &t, ": ", t, ": r");
}
#endif

if (t && (Genode::strcmp(t->name().string(), "pthread.0") == 0)) {
	Genode::raw("rrrrrrrrrr");
}
	bool res = Kernel::restart_thread(native_thread_id(t));

#if 0
for (int i = 0; i < 5; i++) {
Genode::raw(i, ": ", &t, ": ", t, ": rf: ", res);
}
#endif
if (t && (Genode::strcmp(t->name().string(), "pthread.0") == 0)) {
	Genode::raw("ffffffffff");
}
	return res;
}


/**
 * Pause execution of current thread
 */
static inline void thread_stop_myself(Genode::Thread *t) {
//Genode::raw(&t, ": ", t, ": s");

if (t && (Genode::strcmp(t->name().string(), "pthread.0") == 0)) {
	Genode::raw("ssssssssss");
}

#if 0
if (t) {
	if (Genode::strcmp(t->name().string(), "pthread.0") == 0) {
		Genode::raw("thread_stop_myself()");
	}
}
#endif

#if 0
if (t) {
	t->stop_level++;
	if (t->stop_level == 1) {
		Genode::log(&t, ": ", t, ": thread_stop_myself(): ", t->name());
//		for (volatile unsigned long i = 0; i < 100000000; i++) { }
		for (int i = 0; i < 1000; i++) {
			Genode::log(&t, ": s ", i);
		}
		Genode::log(&t, ": stopping");
	} else {
//		Genode::raw(&t, ": stop_level > 1");
		Genode::raw(&t, ": ", t, ": raw: thread_stop_myself(): ", t->name());
	}
}
#endif

//for (volatile unsigned long i = 0; i < 100000000; i++) { }
//Genode::raw(&t, ": s1");
GENODE_TRACE_DURATION_NAMED(0, "thread_stop_myself()");

if (t && (Genode::strcmp(t->name().string(), "pthread.0") == 0)) {
	Genode::raw("tttttttttt");
}

//Genode::raw(&t, ": ", t, ": s2");
//Genode::raw(&t, ": s2");
	Kernel::stop_thread();

if (t && (Genode::strcmp(t->name().string(), "pthread.0") == 0)) {
	Genode::raw("uuuuuuuuuu");
}

//Genode::raw(&t, ": sf");

#if 0
if (t) {
	if (t->stop_level == 1) {
		Genode::log(&t, ": ", t, ": thread_stop_myself() finished: ", t->name());
	} else {
		Genode::raw(&t, ": ", t, ": raw: thread_stop_myself() finished: ", t->name());
	}
	t->stop_level--;
}
#endif

}


#endif /* _INCLUDE__BASE__INTERNAL__LOCK_HELPER_H_ */
