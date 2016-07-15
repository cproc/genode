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
using namespace Sampling_cpu_service;

Cpu_thread_component *
Sampling_cpu_service::Cpu_session_component::lookup_cpu_thread(Thread_capability thread_cap)
{
	for (List_element<Cpu_thread_component> *cpu_thread_element = _thread_list.first();
	     cpu_thread_element;
	     cpu_thread_element = cpu_thread_element->next()) {

		Cpu_thread_component *cpu_thread = cpu_thread_element->object();

		if (cpu_thread->cap() == thread_cap)
			return cpu_thread;
	}

	return 0;
}


Thread_capability
Sampling_cpu_service::Cpu_session_component::create_thread(Pd_session_capability pd,
                                                           Name const &name,
                                                           Affinity::Location affinity,
                                                           Weight weight,
                                                           addr_t utcb)
{
	Cpu_thread_component *cpu_thread = new (_md_alloc) Cpu_thread_component(*this,
	                                                                        pd,
	                                                                        name,
	                                                                        affinity,
	                                                                        weight,
	                                                                        utcb,
	                                                                        name.string(),
	                                                                        _next_thread_id);

	_thread_list.insert(new (_md_alloc) List_element<Cpu_thread_component>(cpu_thread));

	_thread_list_change_handler.thread_list_changed();

	_next_thread_id++;

	return cpu_thread->cap();
}


void Sampling_cpu_service::Cpu_session_component::kill_thread(Thread_capability thread_cap)
{
	for (List_element<Cpu_thread_component> *cpu_thread_element = _thread_list.first();
	     cpu_thread_element;
	     cpu_thread_element = cpu_thread_element->next()) {

		Cpu_thread_component *cpu_thread = cpu_thread_element->object();

		if (cpu_thread->cap() == thread_cap) {
			_thread_list.remove(cpu_thread_element);
			destroy(_md_alloc, cpu_thread_element);
			destroy(_md_alloc, cpu_thread);
			_thread_list_change_handler.thread_list_changed();
			break;
		}
	}

	_parent_cpu_session.kill_thread(thread_cap);
}


void Sampling_cpu_service::Cpu_session_component::exception_sigh(Signal_context_capability handler)
{
	_parent_cpu_session.exception_sigh(handler);
}


Affinity::Space Sampling_cpu_service::Cpu_session_component::affinity_space() const
{
	return _parent_cpu_session.affinity_space();
}


Dataspace_capability
Sampling_cpu_service::Cpu_session_component::trace_control()
{
	return _parent_cpu_session.trace_control();
}


Sampling_cpu_service::Cpu_session_component::Cpu_session_component(
                        Rpc_entrypoint &thread_ep,
                        Allocator *md_alloc,
                        List<List_element<Cpu_thread_component>> &thread_list,
                        Thread_list_change_handler &thread_list_change_handler,
                        const char *args)
: _thread_ep(thread_ep),
  _parent_cpu_session(env()->parent()->session<Cpu_session>(args)),
  _md_alloc(md_alloc),
  _thread_list(thread_list),
  _thread_list_change_handler(thread_list_change_handler),
  _session_label(args),
  _native_cpu_cap(_setup_native_cpu())
{ }


Sampling_cpu_service::Cpu_session_component::~Cpu_session_component()
{
	_cleanup_native_cpu();

	for (List_element<Cpu_thread_component> *cpu_thread_element = _thread_list.first();
	     cpu_thread_element; ) {

		Cpu_thread_component *cpu_thread = cpu_thread_element->object();

		if (cpu_thread->cpu_session_component() == this) {
			_thread_list.remove(cpu_thread_element);
			destroy(_md_alloc, cpu_thread_element);
			destroy(_md_alloc, cpu_thread);
			cpu_thread_element = _thread_list.first();
		} else
			cpu_thread_element = cpu_thread_element->next();
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
	return _native_cpu_cap;
}
