/*
 * \brief  Monitored CPU thread
 * \author Norman Feske
 * \date   2023-05-16
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _MONITORED_THREAD_H_
#define _MONITORED_THREAD_H_

/* Genode includes */
#include <base/rpc_client.h>
#include <cpu_thread/client.h>

/* local includes */
#include <gdb_arch.h>
#include <types.h>

namespace Monitor {
	struct Monitored_thread;
	struct Thread_monitor;
}


/*
 * Interface for the interaction of the monitored thread
 * with the monitor.
 */
struct Monitor::Thread_monitor : Interface
{
	virtual void set_initial_breakpoint(Monitored_thread &thread, addr_t addr,
	                                    char original_instruction[]) = 0;
	virtual void remove_initial_breakpoint(Monitored_thread &thread, addr_t addr,
	                                       char const original_instruction[]) = 0;
	virtual void flush(Monitored_thread &thread) = 0;

	virtual void thread_stopped(Monitored_thread &thread) = 0;
};


struct Monitor::Monitored_thread : Monitored_rpc_object<Cpu_thread>
{
	static void with_thread(Entrypoint &ep, Capability<Cpu_thread> cap,
	                        auto const &monitored_fn, auto const &direct_fn)
	{
		with_monitored<Monitored_thread>(ep, cap, monitored_fn, direct_fn);
	}

	Threads::Element  _threads_elem;
	Thread_monitor   &_thread_monitor;
	bool              _wait;

	addr_t _first_instruction_addr { };
	char   _original_first_instruction[Gdb::breakpoint_len] { };

	Signal_handler<Monitored_thread> _exception_handler;

	bool stopped { false };

	/* values as expected by GDB */
	enum Stop_reason { STOP = 0, TRAP = 5};
	Stop_reason stop_reason { TRAP };

	/* true if the stopped status has not been reported to GDB yet */
	bool stopped_status_pending { false };

	void _handle_exception()
	{
		Genode::log("_handle_exception(): first instruction address: ", Genode::Hex(_first_instruction_addr));

		if (_wait) {
			_wait = false;
			_thread_monitor.remove_initial_breakpoint(*this, _first_instruction_addr,
			                                          _original_first_instruction);

			Thread_state thread_state = _real.call<Rpc_get_state>();

Genode::log("_handle_exception(): trapno: ", Genode::Hex(thread_state.trapno),
            ", ip: ", Genode::Hex(thread_state.ip));

//			thread_state.ip -= 1;

//			_real.call<Rpc_set_state>(thread_state);

//			_real.call<Rpc_resume>();
		}

		stopped = true;
		stopped_status_pending = true;
		stop_reason = TRAP;

		_thread_monitor.thread_stopped(*this);

		Genode::log("_handle_exception() finished");
	}

	using Monitored_rpc_object::Monitored_rpc_object;

	Monitored_thread(Entrypoint &ep, Capability<Cpu_thread> real, Name const &name,
	                 Threads &threads, Threads::Id id,
	                 Thread_monitor &thread_monitor, bool wait)
	:
		Monitored_rpc_object(ep, real, name),
		_threads_elem(*this, threads, id),
		_thread_monitor(thread_monitor), _wait(wait),
		_exception_handler(ep, *this, &Monitored_thread::_handle_exception)
	{
		_real.call<Rpc_exception_sigh>(_exception_handler);

	}

	~Monitored_thread()
	{
		_thread_monitor.flush(*this);
	}

	long unsigned id() const { return _threads_elem.id().value; }

	Dataspace_capability utcb() override {
		return _real.call<Rpc_utcb>(); }

	void start(addr_t ip, addr_t sp) override
	{
Genode::log("start(", Genode::Hex(ip), ", ", Genode::Hex(sp), ")");
		if (_wait) {
			_first_instruction_addr = ip;
			_thread_monitor.set_initial_breakpoint(*this, ip, _original_first_instruction);
		}

		_real.call<Rpc_start>(ip, sp);
	}

	void pause() override
	{
Genode::log("pause()");
		_real.call<Rpc_pause>();
		stopped = true;
		stopped_status_pending = true;
		stop_reason = STOP;
	}

	void resume() override {
Genode::log("resume()");
		stopped = false;
		stopped_status_pending = false;
		_real.call<Rpc_resume>();
	}

	Thread_state state() override {
		return _real.call<Rpc_get_state>(); }

	void state(Thread_state const &state) override {
		_real.call<Rpc_set_state>(state); }

	void exception_sigh(Signal_context_capability handler) override {
		_real.call<Rpc_exception_sigh>(handler); }

	void single_step(bool) override
	{
		Genode::warning("single_step() called for monitored thread");
	}

	void affinity(Affinity::Location location) override {
		_real.call<Rpc_affinity>(location); }

	unsigned trace_control_index() override {
		return _real.call<Rpc_trace_control_index>(); }

	Dataspace_capability trace_buffer() override {
		return _real.call<Rpc_trace_buffer>(); }

	Dataspace_capability trace_policy() override {
		return _real.call<Rpc_trace_policy>(); }
};

#endif /* _MONITORED_THREAD_H_ */
