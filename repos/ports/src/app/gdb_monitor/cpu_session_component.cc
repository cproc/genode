/*
 * \brief  Implementation of the CPU session interface
 * \author Christian Prochaska
 * \date   2011-04-28
 */

/*
 * Copyright (C) 2011-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/env.h>
#include <base/printf.h>
#include <base/sleep.h>
#include <cpu_session_component.h>
#include <util/list.h>

#include <signal.h>

/* GDB monitor includes */
#include "config.h"
#include "thread_info.h"


using namespace Genode;
using namespace Gdb_monitor;


/* mem-break.c */
extern "C" int breakpoint_len;
extern "C" const unsigned char *breakpoint_data;
extern "C" int set_gdb_breakpoint_at(long long where);

/* genode-low.cc */
extern "C" int genode_read_memory(long long memaddr, unsigned char *myaddr, int len);
extern "C" int genode_write_memory (long long memaddr, const unsigned char *myaddr, int len);
extern void genode_remove_thread(unsigned long lwpid);
extern void genode_set_initial_breakpoint_at(long long addr);


static unsigned long new_lwpid = GENODE_MAIN_LWPID;


bool Cpu_session_component::_set_breakpoint_at_first_instruction(addr_t ip)
{
	_breakpoint_ip = ip;

	if (genode_read_memory(_breakpoint_ip, _original_instructions,
	                       breakpoint_len) != 0) {
		PWRN("%s: could not read memory at thread start address", __PRETTY_FUNCTION__);
		return false;
	}

	if (genode_write_memory(_breakpoint_ip, breakpoint_data,
	                        breakpoint_len) != 0) {
		PWRN("%s: could not set breakpoint at thread start address", __PRETTY_FUNCTION__);
		return false;
	}

	return true;
}


void Cpu_session_component::remove_breakpoint_at_first_instruction()
{
#if 1
	Thread_state thread_state = state(thread_cap(GENODE_MAIN_LWPID));
	if (thread_state.ip != _breakpoint_ip)
		PERR("IP: %lx != breakpoint IP: %lx", thread_state.ip, _breakpoint_ip);

	if (genode_write_memory(_breakpoint_ip, _original_instructions,
	                        breakpoint_len) != 0)
		PWRN("%s: could not remove breakpoint at thread start address", __PRETTY_FUNCTION__);
#endif
}


int Cpu_session_component::handle_initial_breakpoint(unsigned long lwpid)
{
	Thread_info *thread_info = _thread_list.first();
	while (thread_info) {
		if (thread_info->lwpid() == lwpid)
			return thread_info->handle_initial_breakpoint();
		thread_info = thread_info->next();
	}
	return 0;
}


Thread_info *Cpu_session_component::_thread_info(Thread_capability thread_cap)
{
	Thread_info *thread_info = _thread_list.first();
	while (thread_info) {
		if (thread_info->thread_cap().local_name() == thread_cap.local_name())
			return thread_info;
		thread_info = thread_info->next();
	}
	return 0;
}


Signal_receiver *Cpu_session_component::exception_signal_receiver()
{
	return _exception_signal_receiver;
}


Thread_capability Cpu_session_component::thread_cap(unsigned long lwpid)
{
	Thread_info *thread_info = _thread_list.first();
	while (thread_info) {
		if (thread_info->lwpid() == lwpid)
			return thread_info->thread_cap();
		thread_info = thread_info->next();
	}
	return Thread_capability();
}


unsigned long Cpu_session_component::lwpid(Thread_capability thread_cap)
{
	return _thread_info(thread_cap)->lwpid();
}


int Cpu_session_component::signal_pipe_read_fd(Thread_capability thread_cap)
{
	return _thread_info(thread_cap)->signal_pipe_read_fd();
}


int Cpu_session_component::send_signal(Thread_capability thread_cap,
                                       int signo,
                                       unsigned long *payload)
{
	Thread_info *thread_info = _thread_info(thread_cap);

	_parent_cpu_session.pause(thread_cap);

	switch (signo) {
		case SIGSTOP:
			PDBG("sending SIGSTOP to thread %lu", thread_info->lwpid());
			Signal_transmitter(thread_info->sigstop_signal_context_cap()).submit();
			return 1;
		case SIGINT:
			Signal_transmitter(thread_info->sigint_signal_context_cap()).submit();
			return 1;
		default:
			PERR("unexpected signal %d", signo);
			return 0;
	}
}


/*
 * This function delivers a SIGSEGV to the first thread with an unresolved
 * page fault that it finds. Multiple page-faulted threads are currently
 * not supported.
 */

void Cpu_session_component::handle_unresolved_page_fault()
{
	/*
	 * It can happen that the thread state of the thread which caused the
	 * page fault is not accessible yet. In that case, we'll retry until
	 * it is accessible.
	 */

	while (1) {

		Thread_capability thread_cap = first();

		while (thread_cap.valid()) {

			try {

				Thread_state thread_state = _parent_cpu_session.state(thread_cap);

				if (thread_state.unresolved_page_fault) {

					/*
					 * On base-foc it is necessary to pause the thread before
					 * IP and SP are available in the thread state.
					 */
					_parent_cpu_session.pause(thread_cap);

					_thread_info(thread_cap)->deliver_signal(SIGSEGV);

					return;
				}

			} catch (Cpu_session::State_access_failed) { }

			thread_cap = next(thread_cap);
		}

	}
}


void Cpu_session_component::stop_new_threads(bool stop)
{
	_stop_new_threads = stop;
}


bool Cpu_session_component::stop_new_threads()
{
	return _stop_new_threads;
}


Lock &Cpu_session_component::stop_new_threads_lock()
{
	return _stop_new_threads_lock;
}


Thread_capability Cpu_session_component::first()
{
	Thread_info *thread_info = _thread_list.first();
	if (thread_info)
		return thread_info->thread_cap();
	else
		return Thread_capability();
}


Thread_capability Cpu_session_component::next(Thread_capability thread_cap)
{
	Thread_info *next_thread_info = _thread_info(thread_cap)->next();
	if (next_thread_info)
		return next_thread_info->thread_cap();
	else
		return Thread_capability();
}


Thread_capability Cpu_session_component::create_thread(Capability<Pd_session> pd,
                                                       Cpu_session::Name const &name,
                                                       Affinity::Location affinity,
                                                       Weight weight,
                                                       addr_t utcb)
{
	/* create thread at parent, keep local copy (needed on NOVA) */
	for (unsigned i = 0; i < MAX_THREADS; i++) {
		if (_threads[i].valid())
			continue;

		Thread_capability cap =
			_parent_cpu_session.create_thread(_core_pd, name, affinity, weight, utcb);
		_threads[i] = cap;

		return cap;
	}

	PERR("maximum number of threads per session reached");
	throw Thread_creation_failed();
}


Ram_dataspace_capability Cpu_session_component::utcb(Thread_capability thread)
{
	return _parent_cpu_session.utcb(thread);
}


void Cpu_session_component::kill_thread(Thread_capability thread_cap)
{
	/* purge local copy of thread capability */
	for (unsigned i = 0; i < MAX_THREADS; i++)
		if (_threads[i].local_name() == thread_cap.local_name())
			_threads[i] = Thread_capability();

	Thread_info *thread_info = _thread_info(thread_cap);

	if (thread_info) {
		genode_remove_thread(thread_info->lwpid());
		_thread_list.remove(thread_info);
		destroy(env()->heap(), thread_info);
	} else
		PERR("%s: could not find thread info for the given thread capability",
		     __PRETTY_FUNCTION__);

	_parent_cpu_session.kill_thread(thread_cap);
}


int Cpu_session_component::start(Thread_capability thread_cap,
                                 addr_t ip, addr_t sp)
{
	Thread_info *thread_info = _thread_info(thread_cap);

PDBG("start(%lx, %lx)", ip, sp);
PDBG("thread_cap.valid(): %u, thread_info: %p", thread_cap.valid(), thread_info);
	if (thread_cap.valid() && !thread_info) {
PDBG("creating thread_info");
		/* valid thread and not started yet */

		thread_info = new (env()->heap())
			Thread_info(this, thread_cap, new_lwpid++, ip);

		/* add the thread to the thread list */
		_thread_list.append(thread_info);

		/* register the exception handler */
		exception_handler(thread_cap,
		                  thread_info->exception_signal_context_cap());

		/* set breakpoint at first instruction */
		if (thread_info->lwpid() == GENODE_MAIN_LWPID)
			_set_breakpoint_at_first_instruction(ip);
		else
			genode_set_initial_breakpoint_at(ip);
	}

	int result = _parent_cpu_session.start(thread_cap, ip, sp);

	if ((result != 0) && thread_info) {
		_thread_list.remove(thread_info);
		destroy(env()->heap(), thread_info);
	}

	PDBG("start() finished");

	return result;
}


void Cpu_session_component::pause(Thread_capability thread_cap)
{
	_parent_cpu_session.pause(thread_cap);
}


void Cpu_session_component::resume(Thread_capability thread_cap)
{
	PDBG("resume()");
#if 1
	_parent_cpu_session.resume(thread_cap);
#endif
}


void Cpu_session_component::cancel_blocking(Thread_capability thread_cap)
{
	_parent_cpu_session.cancel_blocking(thread_cap);
}


void Cpu_session_component::state(Thread_capability thread_cap,
                                 Thread_state const &state)
{
	_parent_cpu_session.state(thread_cap, state);
}


Thread_state Cpu_session_component::state(Thread_capability thread_cap)
{
	return _parent_cpu_session.state(thread_cap);
}


void Cpu_session_component::exception_handler(Thread_capability         thread_cap,
                                              Signal_context_capability sigh_cap)
{
	_parent_cpu_session.exception_handler(thread_cap, sigh_cap);
}


void Cpu_session_component::single_step(Thread_capability thread_cap, bool enable)
{
	_parent_cpu_session.single_step(thread_cap, enable);
}


Affinity::Space Cpu_session_component::affinity_space() const
{
	return _parent_cpu_session.affinity_space();
}


void Cpu_session_component::affinity(Thread_capability thread_cap,
                                     Affinity::Location location)
{
	_parent_cpu_session.affinity(thread_cap, location);
}


Dataspace_capability Cpu_session_component::trace_control()
{
	return _parent_cpu_session.trace_control();
}


unsigned Cpu_session_component::trace_control_index(Thread_capability thread)
{
	return _parent_cpu_session.trace_control_index(thread);
}


Dataspace_capability Cpu_session_component::trace_buffer(Thread_capability thread)
{
	return _parent_cpu_session.trace_buffer(thread);
}


Dataspace_capability Cpu_session_component::trace_policy(Thread_capability thread)
{
	return _parent_cpu_session.trace_policy(thread);
}


Capability<Cpu_session::Native_cpu> Cpu_session_component::native_cpu()
{
	return _parent_cpu_session.native_cpu();
}


Cpu_session_component::Cpu_session_component(Pd_session_capability core_pd,
                                             Signal_receiver *exception_signal_receiver,
                                             const char *args)
: _core_pd(core_pd),
  _parent_cpu_session(env()->parent()->session<Cpu_session>(args)),
  _exception_signal_receiver(exception_signal_receiver)
{
}


Cpu_session_component::~Cpu_session_component()
{
}

int Cpu_session_component::ref_account(Cpu_session_capability) { return -1; }

int Cpu_session_component::transfer_quota(Cpu_session_capability, size_t) { return -1; }

Cpu_session::Quota Cpu_session_component::quota() { return Quota(); }
