/*
 * \brief  Implementation of the CPU session interface
 * \author Christian Prochaska
 * \date   2011-04-28
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/env.h>
#include <base/log.h>
#include <base/sleep.h>
#include <cpu_session_component.h>
#include <util/list.h>

/* GDB monitor includes */
#include "cpu_thread_component.h"

/* libc includes */
#include <sys/signal.h>

using namespace Genode;
using namespace Gdb_monitor;


Cpu_session &Cpu_session_component::parent_cpu_session()
{
	return _parent_cpu_session;
}


Rpc_entrypoint &Cpu_session_component::thread_ep()
{
	return _env.ep().rpc_ep();
}


/*
 * This function delivers a SIGSEGV to the first thread with an unresolved
 * page fault that it finds. Multiple page-faulted threads are currently
 * not supported.
 */

void Cpu_session_component::handle_unresolved_page_fault()
{
Genode::log("Cpu_session_component::handle_unresolved_page_fault()");
	/*
	 * It can happen that the thread state of the thread which caused the
	 * page fault is not accessible yet. In that case, we'll retry until
	 * it is accessible.
	 */

	bool found { false };

	do {

		for_each_thread([&] (Cpu_thread_component &cpu_thread) {

			if (found)
				return;

			try {

				Thread_state thread_state = cpu_thread.state();

				if (thread_state.unresolved_page_fault) {

					/*
					 * On base-foc it is necessary to pause the thread before
					 * IP and SP are available in the thread state.
					 */
					cpu_thread.pause();
					cpu_thread.send_signal(SIGSEGV);

					found = true;
				}

			} catch (Cpu_thread::State_access_failed) { }

		});

	} while (!found);
}


void Cpu_session_component::stop_new_threads(bool stop)
{
	_stop_new_threads = stop;
}


bool Cpu_session_component::stop_new_threads()
{
	return _stop_new_threads;
}


Mutex &Cpu_session_component::stop_new_threads_mutex()
{
	return _stop_new_threads_mutex;
}


int Cpu_session_component::handle_initial_breakpoint(unsigned long lwpid)
{
	int result = 0;

	for_each_thread([&] (Cpu_thread_component &cpu_thread) {
		if (cpu_thread.lwpid() == lwpid)
			result = cpu_thread.handle_initial_breakpoint();
	});

	return result;
}


void Cpu_session_component::pause_all_threads()
{
	Mutex::Guard stop_new_threads_mutex_guard(stop_new_threads_mutex());

	stop_new_threads(true);

	for_each_thread([] (Cpu_thread_component &cpu_thread) {
		cpu_thread.pause(); });
}


void Cpu_session_component::resume_all_threads()
{
	Mutex::Guard stop_new_threads_guard(stop_new_threads_mutex());

	stop_new_threads(false);

	for_each_thread([] (Cpu_thread_component &cpu_thread) {
		cpu_thread.single_step(false);
		cpu_thread.resume();
	});
}


void Cpu_session_component::destroy_thread(unsigned long lwpid)
{
Genode::log("Cpu_session_component::destroy_thread(): ", lwpid);
	for_each_thread([&] (Cpu_thread_component &cpu_thread) {

		if (cpu_thread.lwpid() != lwpid)
			return;

		destroy(_md_alloc, &cpu_thread);
	});
Genode::log("Cpu_session_component::destroy_thread() finished");

	_kill_thread_semaphore.up();
}


Thread_capability Cpu_session_component::create_thread(Capability<Pd_session> pd,
                                                       Cpu_session::Name const &name,
                                                       Affinity::Location affinity,
                                                       Weight weight,
                                                       addr_t utcb)
{
count++;
Genode::log("Cpu_session_component::create_thread(", name, "): count: ", count);
	Cpu_thread_component *cpu_thread =
		new (_md_alloc) Cpu_thread_component(_thread_registry, *this, _core_pd,
		                                     name, affinity, weight, utcb,
		                                     _new_thread_pipe_write_end,
		                                     _breakpoint_len,
		                                     _breakpoint_data,
		                                     _env.ep());

	_env.ep().manage(*cpu_thread);

Genode::log("Cpu_session_component::create_thread() check 1");

	Thread_capability cap = cpu_thread->cap();
Genode::log("Cpu_session_component::create_thread() finished");
	return cap;
}


void Cpu_session_component::kill_thread(Thread_capability thread_cap)
{
	/*
	 * The thread must be destroyed by the main entrypoint
	 * to avoid deadlocks with the thread registry and
	 * monitor jobs.
	 */
Genode::log("Cpu_session_component::kill_thread(): count: ", count);
count--;

	Thread_capability parent_thread_cap;

	for_each_thread([&] (Cpu_thread_component &cpu_thread) {

		if (!(cpu_thread.thread_cap() == thread_cap))
			return;

		parent_thread_cap = cpu_thread.parent_thread_cap();

		/*
		 * Must be called by target ep to avoid a potential
		 * deadlock on base-nova when 'cleanup_call()' would
		 * do an IPC otherwise.
		 */
		_env.ep().dissolve(cpu_thread);

		/*
		 * The signal makes gdbserver remove its internal
		 * thread representation and the 'Cpu_thread_component'
		 * gets destroyed by the receiver as well.
		 */
		cpu_thread.send_signal(SIGCHLD);

Genode::log("Cpu_session_component::kill_thread(): sent SIGCHLD");
	});
//Genode::log("Cpu_session_component::kill_thread(): check");

	/*
	 * Wait until the 'Cpu_thread_component' has been destroyed
	 * to make sure that the parent thread capability is not used
	 * anymore.
	 */

Genode::log("Cpu_session_component::kill_thread(): calling wait_and_dispatch_one_io_signal()");
	_env.ep().wait_and_dispatch_one_io_signal();
Genode::log("Cpu_session_component::kill_thread(): wait_and_dispatch_one_io_signal() returned");

	_kill_thread_semaphore.down();

Genode::log("Cpu_session_component::kill_thread(): calling _parent_cpu_session.kill_thread()");

		_parent_cpu_session.kill_thread(parent_thread_cap);

Genode::log("Cpu_session_component::kill_thread(): _parent_cpu_session.kill_thread() returned");

Genode::log("Cpu_session_component::kill_thread() finished");
}


void Cpu_session_component::exception_sigh(Signal_context_capability handler)
{
	_parent_cpu_session.exception_sigh(handler);
}


Affinity::Space Cpu_session_component::affinity_space() const
{
	return _parent_cpu_session.affinity_space();
}


Dataspace_capability Cpu_session_component::trace_control()
{
	return _parent_cpu_session.trace_control();
}


Capability<Cpu_session::Native_cpu> Cpu_session_component::native_cpu()
{
	return _native_cpu_cap;
}


Cpu_session_component::Cpu_session_component(Env &env,
                                             Allocator &md_alloc,
                                             Pd_session_capability core_pd,
                                             const char *args,
                                             Affinity const &affinity,
                                             int const new_thread_pipe_write_end,
                                             int const breakpoint_len,
                                             unsigned char const *breakpoint_data)
: _env(env),
  _md_alloc(md_alloc),
  _core_pd(core_pd),
  _parent_cpu_session(env.session<Cpu_session>(_id_space_element.id(), args, affinity), *this),
  _new_thread_pipe_write_end(new_thread_pipe_write_end),
  _breakpoint_len(breakpoint_len),
  _breakpoint_data(breakpoint_data),
  _native_cpu_cap(_setup_native_cpu())
{
Genode::log("Cpu_session_component()");
	_env.ep().manage(*this);
}


Cpu_session_component::~Cpu_session_component()
{
	/* 
	 * This destructor is currently not expected to be
	 * called and only partially implemented.
	 *
	 * TODO:
	 *
	 * - destroy all threads (see 'kill_thread()')
	 * - make sure that the component is not accessible
	 *   anymore via child resources
	 */ 

	Genode::error("~Cpu_session_component() called, not implemented yet");

	_cleanup_native_cpu();

	_env.ep().dissolve(*this);
}


int Cpu_session_component::ref_account(Cpu_session_capability) { return -1; }


int Cpu_session_component::transfer_quota(Cpu_session_capability, size_t) { return -1; }


Cpu_session::Quota Cpu_session_component::quota() { return Quota(); }
