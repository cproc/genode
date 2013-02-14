/*
 * \brief  OKL4-specific helper functions for the Lock implementation
 * \author Norman Feske
 * \date   2009-07-09
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

/* OKL4 includes */
namespace Okl4 { extern "C" {
#include <l4/schedule.h>
#include <l4/ipc.h>
} }


/**
 * Yield CPU time
 */
static inline void thread_yield() { Okl4::L4_Yield(); }


static Genode::Native_thread_id main_thread_tid;


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
	using namespace Okl4;

	L4_Word_t dummy;
	L4_ThreadId_t dummy_id;
	L4_ThreadState_t state;

	Genode::Native_thread_id tid = thread_base ?
	                               thread_base->tid().l4id :
	                               main_thread_tid;

	L4_ExchangeRegisters(tid, L4_ExReg_Resume + L4_ExReg_AbortIPC, 0, 0, 0,
	                     0, L4_nilthread, &state.raw, &dummy, &dummy, &dummy,
	                     &dummy, &dummy_id);

	return L4_ThreadWasHalted(state);
}


/*
 * XXX Avoid duplicating this function, see 'ipc.cc', 'pager.cc', and
 *     'irq_session_component.cc'
 */
static inline Genode::Thread_base *thread_get_my_base()
{
	Genode::Thread_base *myself = Genode::Thread_base::myself();
	if (!myself)
		main_thread_tid.raw = Okl4::__L4_TCR_ThreadWord(Genode::UTCB_TCR_THREAD_WORD_MYSELF);
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
	Okl4::L4_ThreadSwitch(tid);
}


/**
 * Unconditionally block the calling thread
 */
static inline void thread_stop_myself()
{
	Genode::Native_thread_id tid;
	tid.raw = Okl4::__L4_TCR_ThreadWord(Genode::UTCB_TCR_THREAD_WORD_MYSELF);
	Okl4::L4_Stop(tid);
}
