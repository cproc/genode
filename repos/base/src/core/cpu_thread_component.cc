/*
 * \brief  Core implementation of the CPU thread interface
 * \author Norman Feske
 * \date   2016-05-10
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* core includes */
#include <cpu_thread_component.h>

using namespace Core;


void Cpu_thread_component::_update_exception_sigh()
{
	Signal_context_capability sigh = _thread_sigh.valid()
	                               ? _thread_sigh : _session_sigh;

	_platform_thread.pager().exception_handler(sigh);
}


void Cpu_thread_component::quota(size_t quota)
{
	_platform_thread.quota(quota);
}


void Cpu_thread_component::session_exception_sigh(Signal_context_capability sigh)
{
	_session_sigh = sigh;
	_update_exception_sigh();
}


void Cpu_thread_component::start(addr_t ip, addr_t sp)
{
	if (_wait_for_trace && 0) {
		_start_ip = ip;
		_start_sp = sp;
		_waiting_for_trace = true;
Genode::raw("start(): ", (void*)_start_ip, ", ", (void*)_start_sp);
	} else
		_platform_thread.start((void *)ip, (void *)sp);
}


void Cpu_thread_component::start_by_trace_monitor()
{
Genode::raw("start_by_trace_monitor(): ", _wait_for_trace, ", ", _waiting_for_trace, ", ", (void*)_start_ip, ", ", (void*)_start_sp);

	if (_wait_for_trace) {

		_wait_for_trace = false;
		_trace_control_slot.control().wait_for_trace(false);

		if (_waiting_for_trace) {
			/* 'start()' has been called already */
			_waiting_for_trace = false;
			_platform_thread.start((void *)_start_ip, (void *)_start_sp);
		}
	}
}


void Cpu_thread_component::pause()
{
	_platform_thread.pause();
}


void Cpu_thread_component::single_step(bool enabled)
{
	_platform_thread.single_step(enabled);
}


void Cpu_thread_component::resume()
{
	_platform_thread.resume();
}


Thread_state Cpu_thread_component::state()
{
	return _platform_thread.state();
}


void Cpu_thread_component::state(Thread_state const &state)
{
	_platform_thread.state(state);
}


void Cpu_thread_component::exception_sigh(Signal_context_capability sigh)
{
	_thread_sigh = sigh;
	_update_exception_sigh();
}


void Cpu_thread_component::affinity(Affinity::Location location)
{
	_platform_thread.affinity(location);
}


unsigned Cpu_thread_component::trace_control_index()
{
	return _trace_control_slot.index;
}


Dataspace_capability Cpu_thread_component::trace_buffer()
{
	return _trace_source.buffer();
}


Dataspace_capability Cpu_thread_component::trace_policy()
{
	return _trace_source.policy();
}

void Cpu_thread_component::trace_start_sigh(Signal_context_capability sigh)
{
	_trace_source.trace_start_sigh(sigh);
}
