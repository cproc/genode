/*
 * \brief  CPU session provided to Noux processes
 * \author Norman Feske
 * \date   2012-02-22
 *
 * The custom implementation of the CPU session interface is used to tweak
 * the startup procedure as performed by the 'Process' class. Normally,
 * processes start execution immediately at creation time at the ELF entry
 * point. For implementing fork semantics, however, this default behavior
 * does not work. Instead, we need to defer the start of the main thread
 * until we have finished copying the address space of the forking process.
 * Furthermore, we need to start the main thread at a custom trampoline
 * function rather than at the ELF entry point. Those customizations are
 * possible by wrapping core's CPU service.
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NOUX__CPU_SESSION_COMPONENT_H_
#define _NOUX__CPU_SESSION_COMPONENT_H_

/* Genode includes */
#include <base/rpc_server.h>
#include <cpu_session/connection.h>

namespace Noux {

	using namespace Genode;

	class Cpu_session_component : public Rpc_object<Cpu_session>
	{
		private:

			bool const        _forked;
			Cpu_connection    _cpu;

			enum { MAX_THREADS = 8, MAIN_THREAD_IDX = 0 };

			Thread_capability _threads[MAX_THREADS];

		public:

			/**
			 * Constructor
			 *
			 * \param forked  false if the CPU session belongs to a child
			 *                created via execve or to the init process, or
			 *                true if the CPU session belongs to a newly forked
			 *                process.
			 *
			 * The 'forked' parameter controls the policy applied to the
			 * startup of the main thread.
			 */
			Cpu_session_component(char const *label, bool forked)
			: _forked(forked), _cpu(label) { }

			/**
			 * Explicitly start main thread, only meaningful when
			 * 'forked' is true
			 */
			void start_main_thread(addr_t ip, addr_t sp)
			{
				_cpu.start(_threads[MAIN_THREAD_IDX], ip, sp);
			}

			Cpu_session_capability cpu_cap() { return _cpu.cap(); }


			/***************************
			 ** Cpu_session interface **
			 ***************************/

			Thread_capability create_thread(size_t weight, Name const &name,
			                                addr_t utcb)
			{
				/* create thread at core, keep local copy (needed on NOVA) */
				for (unsigned i = 0; i < MAX_THREADS; i++) {
					if (_threads[i].valid())
						continue;

					Thread_capability cap =_cpu.create_thread(weight, name, utcb);
					_threads[i] = cap;
					return cap;
				}

				PERR("maximum number of threads per session reached");
				throw Thread_creation_failed();
			}

			Ram_dataspace_capability utcb(Thread_capability thread) {
				return _cpu.utcb(thread); }

			void kill_thread(Thread_capability thread)
			{
				/* purge local copy of thread capability */
				for (unsigned i = 0; i < MAX_THREADS; i++)
					if (_threads[i].local_name() == thread.local_name())
						_threads[i] = Thread_capability();

				_cpu.kill_thread(thread);
			}

			int set_pager(Thread_capability thread,
			              Pager_capability  pager) {
				return _cpu.set_pager(thread, pager); }

			int start(Thread_capability thread, addr_t ip, addr_t sp)
			{
				if (_forked) {
					PINF("defer attempt to start thread at ip 0x%lx", ip);
					return 0;
				}
				return _cpu.start(thread, ip, sp);
			}

			void pause(Thread_capability thread) {
				_cpu.pause(thread); }

			void resume(Thread_capability thread) {
				_cpu.resume(thread); }

			void cancel_blocking(Thread_capability thread) {
				_cpu.cancel_blocking(thread); }

			Thread_state state(Thread_capability thread) {
				return _cpu.state(thread); }

			void state(Thread_capability thread, Thread_state const &state) {
				_cpu.state(thread, state); }

			void exception_handler(Thread_capability         thread,
			                       Signal_context_capability handler) {
				_cpu.exception_handler(thread, handler); }

			void single_step(Thread_capability thread, bool enable) {
				_cpu.single_step(thread, enable); }

			Affinity::Space affinity_space() const {
				return _cpu.affinity_space(); }

			void affinity(Thread_capability thread, Affinity::Location location) {
				_cpu.affinity(thread, location); }

			Dataspace_capability trace_control() {
				return _cpu.trace_control(); }

			unsigned trace_control_index(Thread_capability thread) {
				return _cpu.trace_control_index(thread); }

			Dataspace_capability trace_buffer(Thread_capability thread) {
				return _cpu.trace_buffer(thread); }

			Dataspace_capability trace_policy(Thread_capability thread) {
				return _cpu.trace_policy(thread); }

			Quota quota() override { return _cpu.quota(); }

			int ref_account(Cpu_session_capability c) override {
				return _cpu.ref_account(c); }

			int transfer_quota(Cpu_session_capability c, size_t q) override {
				return _cpu.transfer_quota(c, q); }

			Capability<Native_cpu> native_cpu() override {
				return _cpu.native_cpu(); }
	};
}

#endif /* _NOUX__CPU_SESSION_COMPONENT_H_ */
