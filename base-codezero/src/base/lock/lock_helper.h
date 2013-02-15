/*
 * \brief  Helper functions for the Lock implementation
 * \author Norman Feske
 * \date   2010-04-20
 *
 * For documentation about the interface, please revisit the 'base-pistachio'
 * implementation.
 */

/*
 * Copyright (C) 2010-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/native_types.h>
#include <base/thread.h>

/* Codezero includes */
#include <codezero/syscalls.h>


static Genode::Native_thread_id main_thread_tid;
static Codezero::l4_mutex main_running_lock = { -1 };


static inline void thread_yield()
{
	Codezero::l4_thread_switch(-1);
}


static inline bool thread_check_stopped_and_restart(Genode::Thread_base *thread_base)
{
	Codezero::l4_mutex *running_lock = thread_base ?
	                                   thread_base->utcb()->running_lock() :
	                                   &main_running_lock;
	Codezero::l4_mutex_unlock(running_lock);
	return true;
}


static inline Genode::Thread_base *thread_get_my_base()
{
	Genode::Thread_base *myself = Genode::Thread_base::myself();
	if (!myself && (main_running_lock.lock == -1)) {
		main_thread_tid = Codezero::thread_myself();
		Codezero::l4_mutex_init(&main_running_lock);
		Codezero::l4_mutex_lock(&main_running_lock); /* block on first mutex lock */
	}
	return myself;
}


static inline Genode::Thread_base *thread_invalid_base()
{
	return (Genode::Thread_base*)~0;
}


static inline bool thread_base_valid(Genode::Thread_base *thread_base)
{
	return (thread_base != thread_invalid_base());
}


static inline void thread_switch_to(Genode::Thread_base *thread_base)
{
	Genode::Native_thread_id tid = thread_base ?
	                               thread_base->tid().l4id :
	                               main_thread_tid;
	Codezero::l4_thread_switch(tid);
}


static inline void thread_stop_myself()
{
	Codezero::l4_mutex *running_lock = Genode::Thread_base::myself() ?
	                                   Genode::Thread_base::myself()->utcb()->running_lock() :
	                                   &main_running_lock;
	Codezero::l4_mutex_lock(running_lock);
}
