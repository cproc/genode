/*
 * \brief  Core-specific instance of the CPU session/thread interfaces
 * \author Christian Helmuth
 * \date   2006-07-17
 */

/*
 * Copyright (C) 2006-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CPU_SESSION_COMPONENT_H_
#define _CPU_SESSION_COMPONENT_H_

/* Genode includes */
#include <base/rpc_server.h>
#include <base/thread.h>
#include <cpu_session/client.h>
#include <pd_session/capability.h>

/* GDB monitor includes */
#include "append_list.h"

namespace Gdb_monitor
{
	class Cpu_session_component;
	class Thread_info;
	using namespace Genode;
}

class Gdb_monitor::Cpu_session_component : public Rpc_object<Cpu_session>
{

	private:

		Pd_session_capability _core_pd;

		Cpu_session_client _parent_cpu_session;
		Signal_receiver *_exception_signal_receiver;

		Append_list<Thread_info> _thread_list;

		bool _stop_new_threads = true;
		Lock _stop_new_threads_lock;

		/* data for breakpoint at first instruction */
		enum { MAX_BREAKPOINT_LEN = 8 }; /* value from mem-break.c */
		unsigned char _original_instructions[MAX_BREAKPOINT_LEN];
		addr_t _breakpoint_ip;

		bool _set_breakpoint_at_first_instruction(addr_t ip);

		Thread_info *_thread_info(Thread_capability thread_cap);


		enum { MAX_THREADS = 8, MAIN_THREAD_IDX = 0 };

		Thread_capability _threads[MAX_THREADS];

	public:

		/**
		 * Constructor
		 */
		Cpu_session_component(Pd_session_capability core_pd,
		                      Signal_receiver *exception_signal_receiver,
		                      const char *args);

		/**
		 * Destructor
		 */
		~Cpu_session_component();

		Signal_receiver *exception_signal_receiver();
		Thread_capability thread_cap(unsigned long lwpid);
		unsigned long lwpid(Thread_capability thread_cap);
		int signal_pipe_read_fd(Thread_capability thread_cap);
		int send_signal(Thread_capability thread_cap, int signo, unsigned long *payload);
		void handle_unresolved_page_fault();
		void stop_new_threads(bool stop);
		bool stop_new_threads();
		Lock &stop_new_threads_lock();
		Lock &thread_start_lock();
		Lock &thread_added_to_list_lock();
		void remove_breakpoint_at_first_instruction();
		int handle_initial_breakpoint(unsigned long lwpid);
		Thread_capability first();
		Thread_capability next(Thread_capability);

		/***************************
		 ** CPU session interface **
		 ***************************/

		Thread_capability create_thread(Capability<Pd_session>,
		                                Name const &,
		                                Affinity::Location,
		                                Weight,
		                                addr_t) override;
		Ram_dataspace_capability utcb(Thread_capability thread) override;
		void kill_thread(Thread_capability) override;
		int start(Thread_capability, addr_t, addr_t) override;
		void pause(Thread_capability thread_cap) override;
		void resume(Thread_capability thread_cap) override;
		void cancel_blocking(Thread_capability) override;
		Thread_state state(Thread_capability) override;
		void state(Thread_capability, Thread_state const &) override;
		void exception_handler(Thread_capability         thread,
		                       Signal_context_capability handler) override;
		void single_step(Thread_capability thread, bool enable) override;
		Affinity::Space affinity_space() const override;
		void affinity(Thread_capability, Affinity::Location) override;
		Dataspace_capability trace_control() override;
		unsigned trace_control_index(Thread_capability) override;
		Dataspace_capability trace_buffer(Thread_capability) override;
		Dataspace_capability trace_policy(Thread_capability) override;
		int ref_account(Cpu_session_capability c) override;
		int transfer_quota(Cpu_session_capability c, size_t q) override;
		Quota quota() override;
		Capability<Native_cpu> native_cpu() override;
};

#endif /* _CPU_SESSION_COMPONENT_H_ */
