/*
 * \brief  Implementation of the CPU session interface
 * \author Christian Prochaska
 * \date   2016-01-19
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/env.h>
#include <base/printf.h>
#include "cpu_session_component.h"
#include <util/arg_string.h>
#include <util/list.h>

using namespace Genode;


Thread_capability
Sampling_cpu_service::Cpu_session_component::create_thread(Pd_session_capability pd,
                                                           size_t weight,
                                                           Name const &name,
                                                           Affinity::Location affinity,
                                                           addr_t utcb)
{
	Thread_capability thread_cap =
		_parent_cpu_session.create_thread(pd, weight, name.string(), affinity, utcb);

	Thread_data *thread_data = new (_md_alloc) Thread_data(*this,
	                                                       thread_cap,
	                                                       name.string(),
	                                                       _next_thread_id);

	_thread_list.insert(new (_md_alloc) List_element<Thread_data>(thread_data));

	_thread_list_change_handler.thread_list_changed();

	_next_thread_id++;

	return thread_cap;
}


Ram_dataspace_capability
Sampling_cpu_service::Cpu_session_component::utcb(Thread_capability thread)
{
	return _parent_cpu_session.utcb(thread);
}


void Sampling_cpu_service::Cpu_session_component::kill_thread(Thread_capability thread_cap)
{
	for (List_element<Thread_data> *thread_data_element = _thread_list.first();
	     thread_data_element;
	     thread_data_element = thread_data_element->next()) {

		Thread_data *thread_data = thread_data_element->object();

		if (thread_data->thread_cap() == thread_cap) {
			_thread_list.remove(thread_data_element);
			destroy(_md_alloc, thread_data_element);
			destroy(_md_alloc, thread_data);
			_thread_list_change_handler.thread_list_changed();
			break;
		}
	}

	_parent_cpu_session.kill_thread(thread_cap);
}


int Sampling_cpu_service::Cpu_session_component::start(Thread_capability thread_cap,
                                                       addr_t ip, addr_t sp)
{
	return _parent_cpu_session.start(thread_cap, ip, sp);
}


void Sampling_cpu_service::Cpu_session_component::pause(Thread_capability thread_cap)
{
	_parent_cpu_session.pause(thread_cap);
}


void Sampling_cpu_service::Cpu_session_component::resume(Thread_capability thread_cap)
{
	_parent_cpu_session.resume(thread_cap);
}


void Sampling_cpu_service::Cpu_session_component::cancel_blocking(Thread_capability thread_cap)
{
	_parent_cpu_session.cancel_blocking(thread_cap);
}


void Sampling_cpu_service::Cpu_session_component::state(Thread_capability thread_cap,
                                 Thread_state const &state)
{
	_parent_cpu_session.state(thread_cap, state);
}


Thread_state
Sampling_cpu_service::Cpu_session_component::state(Thread_capability thread_cap)
{
	return _parent_cpu_session.state(thread_cap);
}


void Sampling_cpu_service::Cpu_session_component::exception_handler(Thread_capability thread_cap,
                                                                    Signal_context_capability sigh_cap)
{
	_parent_cpu_session.exception_handler(thread_cap, sigh_cap);
}


void Sampling_cpu_service::Cpu_session_component::single_step(Thread_capability thread_cap,
                                                              bool enable)
{
	_parent_cpu_session.single_step(thread_cap, enable);
}


Affinity::Space Sampling_cpu_service::Cpu_session_component::affinity_space() const
{
	return _parent_cpu_session.affinity_space();
}


void Sampling_cpu_service::Cpu_session_component::affinity(Thread_capability thread_cap,
                                                           Affinity::Location location)
{
	_parent_cpu_session.affinity(thread_cap, location);
}


Dataspace_capability
Sampling_cpu_service::Cpu_session_component::trace_control()
{
	return _parent_cpu_session.trace_control();
}


unsigned Sampling_cpu_service::Cpu_session_component::trace_control_index(Thread_capability thread)
{
	return _parent_cpu_session.trace_control_index(thread);
}


Dataspace_capability
Sampling_cpu_service::Cpu_session_component::trace_buffer(Thread_capability thread)
{
	return _parent_cpu_session.trace_buffer(thread);
}


Dataspace_capability
Sampling_cpu_service::Cpu_session_component::trace_policy(Thread_capability thread)
{
	return _parent_cpu_session.trace_policy(thread);
}


Sampling_cpu_service::Cpu_session_component::Cpu_session_component(
                        Allocator *md_alloc,
                        List<List_element<Thread_data>> &thread_list,
                        Thread_list_change_handler &thread_list_change_handler,
                        const char *args)
: _parent_cpu_session(env()->parent()->session<Cpu_session>(args)),
  _md_alloc(md_alloc),
  _thread_list(thread_list),
  _thread_list_change_handler(thread_list_change_handler),
  _session_label(args)
{ }


Sampling_cpu_service::Cpu_session_component::~Cpu_session_component()
{
PDBG("~Cpu_session_component()");
	for (List_element<Thread_data> *thread_data_element = _thread_list.first();
	     thread_data_element; ) {

		Thread_data *thread_data = thread_data_element->object();

		if (thread_data->cpu_session_component() == this) {
			PDBG("removing thread data: %s", thread_data->label());
			_thread_list.remove(thread_data_element);
			destroy(_md_alloc, thread_data_element);
			destroy(_md_alloc, thread_data);
			thread_data_element = _thread_list.first();
		} else
			thread_data_element = thread_data_element->next();
	}

	_thread_list_change_handler.thread_list_changed();
}


int Sampling_cpu_service::Cpu_session_component::ref_account(Cpu_session_capability cap)
{
	PDBG("not implemented");
	return -1;
}


int Sampling_cpu_service::Cpu_session_component::transfer_quota(Cpu_session_capability cap, size_t size)
{
	PDBG("not implemented");
	return -1;
}


Cpu_session::Quota
Sampling_cpu_service::Cpu_session_component::quota()
{
	PDBG("not implemented");
	return Quota();
}

Capability<Cpu_session::Native_cpu>
Sampling_cpu_service::Cpu_session_component::native_cpu()
{
	return _parent_cpu_session.native_cpu();
}
