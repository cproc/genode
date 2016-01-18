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
#include "cpu_session_component.h"
#include <util/list.h>

using namespace Genode;

void Cpu_session_component::sample()
{
	extern Thread_capability thread_cap;
	//PDBG("calling pause()");
	pause(thread_cap);
	//PDBG("calling state()");
	Thread_state thread_state(state(thread_cap));
	PDBG("IP: %lx", thread_state.ip);
	resume(thread_cap);
	//PDBG("resumed");
}

Thread_capability
Cpu_session_component::create_thread(size_t weight, Name const &name,
                                     addr_t utcb)
{
	PDBG("name: %s", name.string());
	extern Cpu_session_component *cpu_session_component;
	extern Thread_capability thread_cap;
	thread_cap = _parent_cpu_session.create_thread(weight, name.string(), utcb);
	cpu_session_component = this;
	return thread_cap;
}


Ram_dataspace_capability Cpu_session_component::utcb(Thread_capability thread)
{
	return _parent_cpu_session.utcb(thread);
}


void Cpu_session_component::kill_thread(Thread_capability thread_cap)
{
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
	return _parent_cpu_session.start(thread_cap, ip, sp);
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


Cpu_session_component::Cpu_session_component(const char *args)
: _parent_cpu_session(env()->parent()->session<Cpu_session>(args))
{
	PDBG("args: %s", args);
}


Cpu_session_component::~Cpu_session_component()
{
}

int Cpu_session_component::ref_account(Cpu_session_capability) { return -1; }

int Cpu_session_component::transfer_quota(Cpu_session_capability, size_t) { return -1; }

Cpu_session::Quota Cpu_session_component::quota() { return Quota(); }
