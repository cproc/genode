/*
 * \brief  Genode backend for gdbserver
 * \author Christian Prochaska
 * \date   2016-03-07
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


/* Genode includes */
#include <base/printf.h>
#include <cap_session/connection.h>
#include <os/config.h>
#include <ram_session/connection.h>

/* libc includes */
#include <stdio.h>

/* gdbserver includes */
extern "C" {
#define private _private
#include "server.h"
#include "target.h"
#define _private private
}

#include "app_child.h"
#include "rom.h"
#include "signal_handler_thread.h"

#include "genode-low.h"

using namespace Genode;
using namespace Gdb_monitor;

/*
 * C interface implementation
 */

extern "C" {

/* version.c */

const char version[] = "7.3.1";
const char host_name[] = "";

/* genode-low.c */

int using_threads = 1;

static int genode_create_inferior(char *program, char **args)
{
	PDBG("genode_create_inferior(%s)", program);

	/* extract target filename from config file */

	static char filename[32] = "";

	try {
		config()->xml_node().sub_node("target").attribute("name").value(filename, sizeof(filename));
	} catch (Xml_node::Nonexistent_sub_node) {
		PERR("Error: Missing '<target>' sub node.");
		return 0;
	} catch (Xml_node::Nonexistent_attribute) {
		PERR("Error: Missing 'name' attribute of '<target>' sub node.");
		return 0;
	}

	/* extract target node from config file */
	Xml_node target_node = config()->xml_node().sub_node("target");

	/*
	 * preserve the configured amount of memory for gdb_monitor and give the
	 * remainder to the child
	 */
	Number_of_bytes preserved_ram_quota = 0;
	try {
		Xml_node preserve_node = config()->xml_node().sub_node("preserve");
		if (preserve_node.attribute("name").has_value("RAM"))
			preserve_node.attribute("quantum").value(&preserved_ram_quota);
		else
			throw Xml_node::Exception();
	} catch (...) {
		PERR("Error: could not find a valid <preserve> config node");
		return 0;
	}

	Number_of_bytes ram_quota = env()->ram_session()->avail() - preserved_ram_quota;

	/* start the application */
	char *unique_name = filename;
	Capability<Rom_dataspace> file_cap;
	try {
		static Rom_connection rom(filename, unique_name);
		file_cap = rom.dataspace();
	} catch (Rom_connection::Rom_connection_failed) {
		Genode::printf("Error: Could not access file \"%s\" from ROM service.\n", filename);
		return 0;
	}

	/* copy ELF image to writable dataspace */
	Genode::size_t elf_size = Dataspace_client(file_cap).size();
	Capability<Dataspace> elf_cap = clone_rom(file_cap);

	/* create ram session for child with some of our own quota */
	static Ram_connection ram;
	ram.ref_account(env()->ram_session_cap());
	env()->ram_session()->transfer_quota(ram.cap(), (Genode::size_t)ram_quota - elf_size);

	/* cap session for allocating capabilities for parent interfaces */
	static Cap_connection cap_session;

	static Service_registry parent_services;

	enum { CHILD_ROOT_EP_STACK = 1024*sizeof(addr_t) };
	static Rpc_entrypoint child_root_ep(&cap_session, CHILD_ROOT_EP_STACK,
	                                    "child_root_ep");

	static Signal_receiver signal_receiver;

	static Gdb_monitor::Signal_handler_thread
		signal_handler_thread(&signal_receiver);
	signal_handler_thread.start();

	App_child *child = new (env()->heap()) App_child(unique_name,
	                                                 elf_cap,
	                                                 ram.cap(),
	                                                 &cap_session,
	                                                 &parent_services,
	                                                 &child_root_ep,
	                                                 &signal_receiver,
	                                                 target_node);
#if 0
	return child->genode_child_resources();
#endif

	int pid = 0;

	add_process(pid, 1 /* attached */);

	return pid;
}


static ptid_t genode_wait(ptid_t ptid, struct target_waitstatus *status, int options)
{
	PDBG("genode_wait(): ptid: (%d, %ld, %ld), options: %x",
	     ptid.pid, ptid.lwp, ptid.tid, options);
}


#define DUMMY_IMPL(f) \
	static void *dummy_##f() \
	{ \
		fprintf(stderr, "*** " #f "() called, not implemented\n"); \
		return 0; \
	}

DUMMY_IMPL(create_inferior)
DUMMY_IMPL(attach)
DUMMY_IMPL(kill)
DUMMY_IMPL(detach)
DUMMY_IMPL(mourn)
DUMMY_IMPL(join)
DUMMY_IMPL(thread_alive)
DUMMY_IMPL(resume)
DUMMY_IMPL(wait)
DUMMY_IMPL(fetch_registers)
DUMMY_IMPL(store_registers)
DUMMY_IMPL(prepare_to_access_memory)
DUMMY_IMPL(done_accessing_memory)
DUMMY_IMPL(read_memory)
DUMMY_IMPL(write_memory)
DUMMY_IMPL(look_up_symbols)
DUMMY_IMPL(request_interrupt)
DUMMY_IMPL(read_auxv)
DUMMY_IMPL(insert_point)
DUMMY_IMPL(remove_point)
DUMMY_IMPL(stopped_by_watchpoint)
DUMMY_IMPL(stopped_data_address)
DUMMY_IMPL(read_offsets)
DUMMY_IMPL(get_tls_address)
DUMMY_IMPL(qxfer_spu)
DUMMY_IMPL(hostio_last_error)
DUMMY_IMPL(qxfer_osdata)
DUMMY_IMPL(qxfer_siginfo)
DUMMY_IMPL(supports_non_stop)
DUMMY_IMPL(async)
DUMMY_IMPL(start_non_stop)
DUMMY_IMPL(supports_multi_process)
DUMMY_IMPL(handle_monitor_command)
DUMMY_IMPL(core_of_thread)
DUMMY_IMPL(process_qsupported)
DUMMY_IMPL(supports_tracepoints)
DUMMY_IMPL(read_pc)
DUMMY_IMPL(write_pc)
DUMMY_IMPL(thread_stopped)
DUMMY_IMPL(get_tib_address)
DUMMY_IMPL(pause_all)
DUMMY_IMPL(unpause_all)
DUMMY_IMPL(cancel_breakpoints)
DUMMY_IMPL(stabilize_threads)
DUMMY_IMPL(install_fast_tracepoint_jump_pad)
DUMMY_IMPL(emit_ops)

#define DUMMY(f) dummy_##f
#define DUMMY_NULL(f) NULL

static struct target_ops genode_target_ops = {
  genode_create_inferior,
  DUMMY(attach),
  DUMMY(kill),
  DUMMY(detach),
  DUMMY(mourn),
  DUMMY(join),
  DUMMY(thread_alive),
  DUMMY(resume),
  genode_wait,
  DUMMY(fetch_registers),
  DUMMY(store_registers),
  DUMMY(prepare_to_access_memory),
  DUMMY(done_accessing_memory),
  DUMMY(read_memory),
  DUMMY(write_memory),
  DUMMY(look_up_symbols),
  DUMMY(request_interrupt),
  DUMMY(read_auxv),
  DUMMY(insert_point),
  DUMMY(remove_point),
  DUMMY(stopped_by_watchpoint),
  DUMMY(stopped_data_address),
  DUMMY(read_offsets),
  DUMMY(get_tls_address),
  DUMMY(qxfer_spu),
  DUMMY(hostio_last_error),
  DUMMY(qxfer_osdata),
  DUMMY(qxfer_siginfo),
  DUMMY(supports_non_stop),
  DUMMY(async),
  DUMMY(start_non_stop),
  DUMMY(supports_multi_process),
  DUMMY(handle_monitor_command),
  DUMMY(core_of_thread),
  DUMMY(process_qsupported),
  DUMMY_NULL(supports_tracepoints),
  DUMMY(read_pc),
  DUMMY(write_pc),
  DUMMY(thread_stopped),
  DUMMY(get_tib_address),
  DUMMY(pause_all),
  DUMMY(unpause_all),
  DUMMY(cancel_breakpoints),
  DUMMY(stabilize_threads),
  NULL, /* install_fast_tracepoint_jump_pad */
  DUMMY(emit_ops),
};


void initialize_low()
{
	fprintf(stderr, "initialize_low()\n");

	set_target_ops(&genode_target_ops);

#if 0
	set_breakpoint_data (the_low_target.breakpoint,
	                     the_low_target.breakpoint_len);
#endif

#if 0 /* FIXME - see linux-low.c */
	the_low_target.arch_setup();
#endif
}

} /* extern "C" */
