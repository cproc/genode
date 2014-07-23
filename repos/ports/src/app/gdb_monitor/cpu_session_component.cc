/*
 * \brief  Implementation of the CPU session interface
 * \author Christian Prochaska
 * \date   2011-04-28
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
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

extern void genode_add_thread(unsigned long lwpid);
extern void genode_remove_thread(unsigned long lwpid);

using namespace Genode;
using namespace Gdb_monitor;

/* FIXME: use an allocator */
static unsigned long new_lwpid = GENODE_LWP_BASE;


Thread_info *Cpu_session_component::_thread_info(Thread_capability thread_cap)
{
	Thread_info *thread_info = _thread_list.first();
	while (thread_info) {
		if (thread_info->thread_cap().local_name() == thread_cap.local_name()) {
			return thread_info;
			break;
		}
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
		if (thread_info->lwpid() == lwpid) {
			return thread_info->thread_cap();
		}
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
PDBG("calling pause()");
	_parent_cpu_session.pause(thread_cap);
PDBG("pause returned");
	switch (signo) {
		case SIGSTOP:
			Signal_transmitter(thread_info->sigstop_signal_context_cap()).submit();
			return 1;
		case SIGINT:
			Signal_transmitter(thread_info->sigint_signal_context_cap()).submit();
			return 1;
		case SIGINFO:
			Signal_transmitter(thread_info->siginfo_signal_context_cap()).submit();
			PDBG("sent SIGINFO");
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

					_thread_info(thread_cap)->deliver_signal(SIGSEGV, 0);

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


Thread_capability Cpu_session_component::create_thread(Cpu_session::Name const &name, addr_t utcb)
{
	Thread_capability thread_cap =
		_parent_cpu_session.create_thread(name.string(), utcb);

	if (thread_cap.valid()) {
		Thread_info *thread_info = new (env()->heap())
			Thread_info(this, thread_cap, new_lwpid++);
		_thread_list.append(thread_info);
	} else
		PERR("%s: thread creation failed", __PRETTY_FUNCTION__);

	return thread_cap;
}


Ram_dataspace_capability Cpu_session_component::utcb(Thread_capability thread)
{
	return _parent_cpu_session.utcb(thread);
}


void Cpu_session_component::kill_thread(Thread_capability thread_cap)
{
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


int Cpu_session_component::set_pager(Thread_capability thread_cap,
                                     Pager_capability  pager_cap)
{
	return _parent_cpu_session.set_pager(thread_cap, pager_cap);
}


int Cpu_session_component::start(Thread_capability thread_cap,
                                 addr_t ip, addr_t sp)
{
	Thread_info *thread_info = _thread_info(thread_cap);

	if (thread_info) {

		/* register the exception handler */
		exception_handler(thread_cap,
		                  thread_info->exception_signal_context_cap());

		/* make the thread stop at the second instruction */
		single_step(thread_cap, true);

	} else
		PERR("%s: could not find thread info for the given thread capability",
		     __PRETTY_FUNCTION__);

	int result = _parent_cpu_session.start(thread_cap, ip, sp);

	if ((result == 0) && thread_info) {
		/* inform gdbserver about the new thread */
		genode_add_thread(thread_info->lwpid());
	}

	return result;
}


void Cpu_session_component::pause(Thread_capability thread_cap)
{
	_parent_cpu_session.pause(thread_cap);
}


void Cpu_session_component::resume(Thread_capability thread_cap)
{
	_parent_cpu_session.resume(thread_cap);
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


Cpu_session_component::Cpu_session_component(Signal_receiver *exception_signal_receiver, const char *args)
: _parent_cpu_session(env()->parent()->session<Cpu_session>(args)),
  _exception_signal_receiver(exception_signal_receiver)
{
}


Cpu_session_component::~Cpu_session_component()
{
}
