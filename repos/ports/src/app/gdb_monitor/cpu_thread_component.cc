/*
 * \brief  Cpu_thread_component class for GDB monitor
 * \author Christian Prochaska
 * \date   2016-05-12
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */


/* GDB monitor includes */
#include "cpu_thread_component.h"
#include "genode-low.h"

/* libc includes */
#include <signal.h>
#include <unistd.h>
#include <libc/component.h>


static unsigned long new_lwpid = GENODE_MAIN_LWPID;


using namespace Gdb_monitor;


bool Cpu_thread_component::_set_breakpoint_at_first_instruction(addr_t ip)
{
	_breakpoint_ip = ip;

	if (genode_read_memory(_breakpoint_ip, _original_instructions,
	                       _breakpoint_len) != 0) {
		warning(__PRETTY_FUNCTION__, ": could not read memory at thread start address");
		return false;
	}

	if (genode_write_memory(_breakpoint_ip, _breakpoint_data,
	                        _breakpoint_len) != 0) {
		warning(__PRETTY_FUNCTION__, ": could not set breakpoint at thread start address");
		return false;
	}

	return true;
}


void Cpu_thread_component::_remove_breakpoint_at_first_instruction()
{
	if (genode_write_memory(_breakpoint_ip, _original_instructions,
	                        _breakpoint_len) != 0)
		warning(__PRETTY_FUNCTION__, ": could not remove breakpoint at thread start address");
}


void Cpu_thread_component::_handle_exception()
{
Genode::log("_handle_exception()");
	_deliver_signal(SIGTRAP);
Genode::log("_handle_exception() finished");
}


void Cpu_thread_component::_handle_sigstop()
{
Genode::log("_handle_sigstop()");

	_deliver_signal(SIGSTOP);
}


void Cpu_thread_component::_handle_sigint()
{
Genode::log("_handle_sigint");
	_deliver_signal(SIGINT);
}


void Cpu_thread_component::_handle_sigsegv()
{
Genode::log("_handle_sigsegv");
	_deliver_signal(SIGSEGV);
}


void Cpu_thread_component::_handle_sigchld()
{
Genode::log("_handle_sigchld");
	_deliver_signal(SIGCHLD);
}


Cpu_thread_component::Cpu_thread_component(Registry<Cpu_thread_component> &registry,
                                           Cpu_session_component          &cpu_session_component,
		                                   Capability<Pd_session>          pd,
                                           Cpu_session::Name const        &name,
                                           Affinity::Location              affinity,
                                           Cpu_session::Weight             weight,
                                           addr_t                          utcb,
                                           int const                       new_thread_pipe_write_end,
                                           int const                       breakpoint_len,
                                           unsigned char const            *breakpoint_data,
                                           Entrypoint                     &signal_ep)
:   Registry<Cpu_thread_component>::Element(registry, *this),
	_cpu_session_component(cpu_session_component),
	_parent_cpu_thread(
		_cpu_session_component.parent_cpu_session().create_thread(pd,
		                                                          name,
		                                                          affinity,
		                                                          weight,
		                                                          utcb)),
	_new_thread_pipe_write_end(new_thread_pipe_write_end),
	_breakpoint_len(breakpoint_len),
	_breakpoint_data(breakpoint_data),
	_lwpid(new_lwpid++),
	_exception_handler(signal_ep, *this,
	                   &Cpu_thread_component::_handle_exception),
	_sigstop_handler(signal_ep, *this,
	                 &Cpu_thread_component::_handle_sigstop),
	_sigint_handler(signal_ep, *this,
	                &Cpu_thread_component::_handle_sigint),
	_sigsegv_handler(signal_ep, *this,
	                 &Cpu_thread_component::_handle_sigsegv),
	_sigchld_handler(signal_ep, *this,
	                 &Cpu_thread_component::_handle_sigchld)
{
Genode::log("Cpu_thread_component(", _lwpid, "): ", name);
	if (pipe(_pipefd) != 0)
		error("could not create pipe");
Genode::log("Cpu_thread_component() finished");
}


Cpu_thread_component::~Cpu_thread_component()
{
Genode::log("~Cpu_thread_component(", _lwpid, ")");
	close(_pipefd[0]);
Genode::log("~Cpu_thread_component(", _lwpid, "): check 1");
	close(_pipefd[1]);
Genode::log("~Cpu_thread_component(", _lwpid, ") finished");
}


int Cpu_thread_component::send_signal(int signo)
{
	pause();

	switch (signo) {
		case SIGSTOP:
			if (_verbose)
				Genode::log("send_signal(SIGSTOP)");
			_sigstop_handler.local_submit();
			return 0;
		case SIGINT:
			if (_verbose)
				Genode::log("send_signal(SIGINT)");
			_sigint_handler.local_submit();
			return 0;
		case SIGCHLD:
			if (_verbose)
				Genode::log("send_signal(SIGCHLD)");
			
			_sigchld_handler.local_submit();
			return 0;
		case SIGSEGV:
			if (_verbose)
				Genode::log("send_signal(SIGSEGV)");
			_sigsegv_handler.local_submit();
			return 0;
		default:
			error("unexpected signal ", signo);
			return -1;
	}
}


void Cpu_thread_component::_deliver_signal(int signo)
{
	if ((signo == SIGTRAP) && _initial_sigtrap_pending) {

		_initial_sigtrap_pending = false;

		if (_verbose)
			log("received initial SIGTRAP for lwpid ", _lwpid);

		if (_lwpid == GENODE_MAIN_LWPID) {
			_remove_breakpoint_at_first_instruction();
			_initial_breakpoint_handled = true;
		}

		/*
		 * The mutex guard prevents an interruption by
		 * 'genode_stop_all_threads()', which could cause
		 * the new thread to be resumed when it should be
		 * stopped.
		 */

		Mutex::Guard stop_new_threads_mutex_guard(
			_cpu_session_component.stop_new_threads_mutex());

		if (!_cpu_session_component.stop_new_threads())
			resume();

		/*
		 * gdbserver expects SIGSTOP as first signal of a new thread,
		 * but we cannot write SIGSTOP here, because waitpid() would
		 * detect that the thread is in an exception state and wait
		 * for the SIGTRAP. So SIGINFO ist used for this purpose.
		 */
		signo = SIGINFO;
	}

	switch (signo) {
		case SIGSTOP:
			if (_verbose)
				log("delivering SIGSTOP to thread ", _lwpid);
			break;
		case SIGTRAP:
			if (_verbose)
				log("delivering SIGTRAP to thread ", _lwpid);
			break;
		case SIGSEGV:
			if (_verbose)
				log("delivering SIGSEGV to thread ", _lwpid);
			break;
		case SIGINT:
			if (_verbose)
				log("delivering SIGINT to thread ", _lwpid);
			break;
		case SIGINFO:
			if (_verbose)
				if (_lwpid != GENODE_MAIN_LWPID)
					log("delivering initial SIGSTOP to thread ", _lwpid);
			break;
		case SIGCHLD:
			if (_verbose)
				log("delivering SIGCHLD to thread ", _lwpid);
			break;
		default:
			error("unexpected signal ", signo);
	}

	if (!((signo == SIGINFO) && (_lwpid == GENODE_MAIN_LWPID)))
		write(_pipefd[1], &signo, sizeof(signo));

	/*
	 * gdbserver might be blocking in 'waitpid()' without having
	 * the new thread's pipe fd in its 'select' fd set yet. Writing
	 * into the 'new thread pipe' here will unblock 'select' in this
	 * case.
	 */
	if (signo == SIGINFO)
		write(_new_thread_pipe_write_end, &_lwpid, sizeof(_lwpid));
}

Dataspace_capability Cpu_thread_component::utcb()
{
Genode::log("Cpu_thread_component::utcb()");
	return _parent_cpu_thread.utcb();
}


void Cpu_thread_component::start(addr_t ip, addr_t sp)
{
Genode::log("Cpu_thread_component::start(", _lwpid, "): ip: ", Genode::Hex(ip), ", sp: ", Genode::Hex(sp));
	_initial_ip = ip;

	/* register the exception handler */
	exception_sigh(_exception_handler);
Genode::log("Cpu_thread_component::start(", _lwpid, ") check 1");

	/* set breakpoint at first instruction */

	if (lwpid() == GENODE_MAIN_LWPID) {
Genode::log("Cpu_thread_component::start(", _lwpid, ") check 1.1");
		_set_breakpoint_at_first_instruction(ip);
	} else {
Genode::log("Cpu_thread_component::start(", _lwpid, ") check 1.2");
		genode_set_initial_breakpoint_at(ip);
	}

Genode::log("Cpu_thread_component::start(", _lwpid, ") check 2");

	_parent_cpu_thread.start(ip, sp);
Genode::log("Cpu_thread_component::start(", _lwpid, ") finished");
}


void Cpu_thread_component::pause()
{
Genode::log("Cpu_thread_component::pause()");

	unsigned loop_cnt = 0;

	/* required semantic for gdb is that thread is paused with valid state */
	for (;;) {
Genode::log("Cpu_thread_component::pause() calling _parent_cpu_thread.pause()");

		_parent_cpu_thread.pause();
Genode::log("Cpu_thread_component::pause(): _parent_cpu_thread.pause() returned");

		try {
			Genode::log("Cpu_thread_component::pause(): calling _parent_cpu_thread.state()");
			/* check if the thread state is valid */
			_parent_cpu_thread.state();
			/* the thread is paused */
			Genode::log("Cpu_thread_component::pause(): _parent_cpu_thread.state() returned");
			return;
		} catch (State_access_failed) {
			loop_cnt ++;

			if (loop_cnt % 100 == 0)
				Genode::warning("pausing thread failed ", loop_cnt,
				                ". times, continue looping");
		}
	}
Genode::log("Cpu_thread_component::pause() finished");
}


void Cpu_thread_component::resume()
{
Genode::log("Cpu_thread_component::resume(", _lwpid, ")");
	_parent_cpu_thread.resume();
Genode::log("Cpu_thread_component::resume(", _lwpid, ") finished");
}


void Cpu_thread_component::single_step(bool enable)
{
	_parent_cpu_thread.single_step(enable);
}


void Cpu_thread_component::state(Thread_state const &state)
{
	_parent_cpu_thread.state(state);
}


Thread_state Cpu_thread_component::state()
{
//	Genode::log("Cpu_thread_component::state(): calling _parent_cpu_thread.state()");
	Thread_state res = _parent_cpu_thread.state();
//	Genode::log("Cpu_thread_component::state(): _parent_cpu_thread.state() returned");
	return res;
}


void Cpu_thread_component::exception_sigh(Signal_context_capability handler)
{
	_parent_cpu_thread.exception_sigh(handler);
}


void Cpu_thread_component::affinity(Affinity::Location location)
{
	_parent_cpu_thread.affinity(location);
}


unsigned Cpu_thread_component::trace_control_index()
{
	return _parent_cpu_thread.trace_control_index();
}


Dataspace_capability Cpu_thread_component::trace_buffer()
{
	return _parent_cpu_thread.trace_buffer();
}


Dataspace_capability Cpu_thread_component::trace_policy()
{
	return _parent_cpu_thread.trace_policy();
}
