/*
 * \brief  Client-side cpu session NOVA extension
 * \author Alexander Boettcher
 * \date   2012-07-27
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__CPU_SESSION__CLIENT_H_
#define _INCLUDE__CPU_SESSION__CLIENT_H_

#include <base/rpc_client.h>
#include <base/thread.h>

#include <cpu_session/cpu_session.h>

namespace Genode {

	struct Cpu_session_client : Rpc_client<Cpu_session>
	{
		explicit Cpu_session_client(Cpu_session_capability session)
		: Rpc_client<Cpu_session>(static_cap_cast<Cpu_session>(session)) { }

		Thread_capability
		create_thread(Capability<Pd_session> pd, size_t quota, Name const &name,
		              Affinity::Location affinity, addr_t utcb = 0) override {
			return call<Rpc_create_thread>(pd, quota, name, affinity, utcb); }

		Ram_dataspace_capability utcb(Thread_capability thread) {
			return call<Rpc_utcb>(thread); }

		void kill_thread(Thread_capability thread) {
			call<Rpc_kill_thread>(thread); }

		int start(Thread_capability thread, addr_t ip, addr_t sp) {
			return call<Rpc_start>(thread, ip, sp); }

		void pause(Thread_capability thread)
		{
			for (;;) {

				call<Rpc_pause>(thread);

				try {
					/* check if the thread state is valid */
					state(thread);
					/* the thread is blocked in the kernel */
					return;
				} catch (State_access_failed) {
					/* the thread is (most likely) running on a different CPU */
				}
			}
		}

		void resume(Thread_capability thread) {
			call<Rpc_resume>(thread); }

		void cancel_blocking(Thread_capability thread) {
			call<Rpc_cancel_blocking>(thread); }

		Thread_state state(Thread_capability thread) {
			return call<Rpc_get_state>(thread); }

		void state(Thread_capability thread, Thread_state const &state) {
			call<Rpc_set_state>(thread, state); }

		void exception_handler(Thread_capability thread, Signal_context_capability handler) {
			call<Rpc_exception_handler>(thread, handler); }

		void single_step(Thread_capability thread, bool enable) {
			call<Rpc_single_step>(thread, enable); }

		Affinity::Space affinity_space() const {
			return call<Rpc_affinity_space>(); }

		void affinity(Thread_capability thread, Affinity::Location location) {
			call<Rpc_affinity>(thread, location); }

		Dataspace_capability trace_control() {
			return call<Rpc_trace_control>(); }

		unsigned trace_control_index(Thread_capability thread) {
			return call<Rpc_trace_control_index>(thread); }

		Dataspace_capability trace_buffer(Thread_capability thread) {
			return call<Rpc_trace_buffer>(thread); }

		Dataspace_capability trace_policy(Thread_capability thread) {
			return call<Rpc_trace_policy>(thread); }

		int ref_account(Cpu_session_capability session) {
			return call<Rpc_ref_account>(session); }

		int transfer_quota(Cpu_session_capability session, size_t amount) {
			return call<Rpc_transfer_quota>(session, amount); }

		Quota quota() override { return call<Rpc_quota>(); }

		Capability<Native_cpu> native_cpu() override { return call<Rpc_native_cpu>(); }
	};
}

#endif /* _INCLUDE__CPU_SESSION__CLIENT_H_ */
