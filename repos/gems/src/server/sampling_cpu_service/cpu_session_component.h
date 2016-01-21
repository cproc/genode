/*
 * \brief  CPU session component interface 
 * \author Christian Prochaska
 * \date   2016-01-18
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CPU_SESSION_COMPONENT_H_
#define _CPU_SESSION_COMPONENT_H_

/* Genode includes */
#include <base/allocator.h>
#include <base/rpc_server.h>
#include <cpu_session/client.h>
#include <os/session_policy.h>

/* local includes */
#include "thread_data.h"
#include "thread_list_change_handler.h"

namespace Sampling_cpu_service
{
	using namespace Genode;
	class Cpu_session_component;
}

class Sampling_cpu_service::Cpu_session_component : public Rpc_object<Cpu_session>
{
	private:

		Cpu_session_client               _parent_cpu_session;
		Allocator                       *_md_alloc;
		List<List_element<Thread_data>> &_thread_list;
		Thread_list_change_handler      &_thread_list_change_handler;
		Session_label                    _session_label;
		unsigned int                     _next_thread_id = 0;

	public:

		Session_label &session_label() { return _session_label; }
		Cpu_session_capability parent_cpu_session_cap() { return _parent_cpu_session; }

		/**
		 * Constructor
		 */
		Cpu_session_component(Allocator *md_alloc,
		                      List<List_element<Thread_data>> &thread_list,
		                      Thread_list_change_handler &thread_list_change_handler,
		                      const char *args);

		/**
		 * Destructor
		 */
		~Cpu_session_component();


		/***************************
		 ** CPU session interface **
		 ***************************/

		Thread_capability create_thread(size_t, Name const &, addr_t) override;
		Ram_dataspace_capability utcb(Thread_capability thread) override;
		void kill_thread(Thread_capability) override;
		int set_pager(Thread_capability, Pager_capability) override;
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
};

#endif /* _CPU_SESSION_COMPONENT_H_ */
