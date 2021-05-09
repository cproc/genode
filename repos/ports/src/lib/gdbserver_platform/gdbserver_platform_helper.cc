/*
 * \brief  Genode backend for GDBServer - helper functions
 * \author Christian Prochaska
 * \date   2011-07-07
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <cpu_thread/client.h>

#include "cpu_session_component.h"
#include "genode_child_resources.h"

#include "server.h"
#include "linux-low.h"
#include "genode-low.h"

using namespace Genode;
using namespace Gdb_monitor;

extern Genode_child_resources *genode_child_resources();


static constexpr bool verbose = false;


Thread_state get_current_thread_state()
{
	Cpu_session_component &csc = genode_child_resources()->cpu_session_component();

	ptid_t ptid = current_thread->id;

	Cpu_thread_client cpu_thread(csc.thread_cap(ptid.lwp()));
Genode::log("get_current_thread_state(): calling cpu_thread.state()");
	Thread_state res = cpu_thread.state();
Genode::log("get_current_thread_state(): cpu_thread.state() returned");
	return res;
}


void set_current_thread_state(Thread_state thread_state)
{
	Cpu_session_component &csc = genode_child_resources()->cpu_session_component();

	ptid_t ptid = current_thread->id;

	Cpu_thread_client cpu_thread(csc.thread_cap(ptid.lwp()));

	cpu_thread.state(thread_state);
}


void fetch_register(const char *reg_name,
                           addr_t thread_state_reg,
                           unsigned long &value)
{
	value = thread_state_reg;

	if (verbose)
		log(__func__, ": ", reg_name, " = ", Hex(value));
}


void cannot_fetch_register(const char *reg_name)
{
	if (verbose)
		log("cannot fetch register ", reg_name);
}


bool store_register(const char *reg_name,
                    addr_t &thread_state_reg,
                    unsigned long value)
{
	if (verbose)
		log(__func__, ": ", reg_name, " = ", Hex(value));

	if (thread_state_reg == value)
		return false;

	thread_state_reg = value;

	return true;
}


void cannot_store_register(const char *reg_name, unsigned long value)
{
	if (verbose)
		log("cannot set contents of register ", reg_name, " (", Hex(value), ")");
}
