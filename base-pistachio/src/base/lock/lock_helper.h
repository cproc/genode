/*
 * \brief  Pistachio-specific helper functions for the Lock implementation
 * \author Norman Feske
 * \date   2009-07-15
 *
 * This file serves as adapter between the generic lock implementation
 * in 'lock.cc' and the underlying kernel.
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
#include <base/snprintf.h>

/* Pistachio includes */
namespace Pistachio {
#include <l4/schedule.h>
#include <l4/ipc.h>
}
extern "C" int raw_write_str(const char *str);

static Genode::Native_thread_id main_thread_tid;


/**
 * Yield CPU time
 */
static inline void thread_yield() { Pistachio::L4_Yield(); }


/**
 * Custom ExchangeRegisters wrapper for waking up a thread
 *
 * When waking up an lock applicant, we need to make sure that the thread was
 * stopped beforehand. Therefore, we evaluate the previous thread state as
 * returned by the 'L4_ExchangeRegisters' call.
 *
 * \return true if the thread was in blocking state
 */
static inline bool thread_check_stopped_and_restart(Genode::Thread_base *thread_base)
{
	using namespace Pistachio;

	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	Genode::Native_thread_id tid = thread_base ?
	                               thread_base->tid().l4id :
	                               main_thread_tid;
	char buf[128];
	Genode::snprintf(buf, sizeof(buf), "%p: thread_check_stopped_and_restart(%p): l4id = %lu\n", Genode::Thread_base::myself(), thread_base, tid.raw);
	raw_write_str(buf);

	enum { RESUME = 1 << 8, CANCEL_IPC = 3 << 1 };
	L4_ExchangeRegisters(tid, RESUME | CANCEL_IPC, 0, 0, 0,
	                     0, L4_nilthread, &state.raw, &dummy, &dummy, &dummy,
	                     &dummy, &dummy_id);

	return L4_ThreadWasHalted(state);
}


static inline Genode::Thread_base *thread_get_my_base()
{
	Genode::Thread_base *myself = Genode::Thread_base::myself();
	if (!myself)
		main_thread_tid = Pistachio::L4_Myself();
#if 0
	char buf[128];
	Genode::snprintf(buf, sizeof(buf), "%p: thread_get_my_base(): l4id = %lu\n", myself, Pistachio::L4_Myself().raw);
	raw_write_str(buf);
#endif
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


/**
 * Yield CPU time to the specified thread
 */
static inline void thread_switch_to(Genode::Thread_base *thread_base)
{
	Genode::Native_thread_id tid = thread_base ?
	                               thread_base->tid().l4id :
	                               main_thread_tid;
	char buf[128];
	Genode::snprintf(buf, sizeof(buf), "%p: thread_switch_to(%p): l4id = %lu\n", Genode::Thread_base::myself(), thread_base, tid.raw);
	raw_write_str(buf);

	Pistachio::L4_ThreadSwitch(tid);
}


/**
 * Unconditionally block the calling thread
 */
static inline void thread_stop_myself()
{
	char buf[128];
	Genode::snprintf(buf, sizeof(buf), "%p: thread_stop_myself(): l4id = %lu\n", Genode::Thread_base::myself(), Pistachio::L4_Myself().raw);
	raw_write_str(buf);
	if (!Genode::Thread_base::myself())
		main_thread_tid = Pistachio::L4_Myself();
	Pistachio::L4_Stop(Pistachio::L4_Myself());
}
