/*
 * \brief  Genode backend for GDBServer (C++)
 * \author Christian Prochaska
 * \author Norman Feske
 * \date   2011-05-06
 */

/*
 * Copyright (C) 2011-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <signal.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <unistd.h>

extern "C" {
#define private _private
#include "genode-low.h"
#include "server.h"
#include "linux-low.h"
#define _private private

int linux_detach_one_lwp (struct inferior_list_entry *entry, void *args);
}

#include <base/printf.h>
#include <base/service.h>
#include <cap_session/connection.h>
#include <dataspace/client.h>
#include <os/config.h>
#include <ram_session/connection.h>
#include <rom_session/connection.h>
#include <util/xml_node.h>

#include "app_child.h"
#include "cpu_session_component.h"
#include "genode_child_resources.h"
#include "rom.h"
#include "signal_handler_thread.h"

static bool verbose = true;

static int _new_thread_pipe[2];

/*
 * When 'waitpid()' reports a SIGTRAP, this variable stores the lwpid of the
 * corresponding thread. This information is used in the initial breakpoint
 * handler to let the correct thread handle the event.
 */
static unsigned long sigtrap_lwpid;

using namespace Genode;
using namespace Gdb_monitor;

static Genode_child_resources *_genode_child_resources = 0;


Genode_child_resources *genode_child_resources()
{
	return _genode_child_resources;
}


static void genode_stop_thread(unsigned long lwpid)
{
PDBG("genode_stop_thread(%lu)", lwpid);
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->thread_cap(lwpid);

	if (!thread_cap.valid()) {
		PERR("%s: could not find thread capability for lwpid %lu",
		     __PRETTY_FUNCTION__, lwpid);
		return;
	}

	csc->pause(thread_cap);
}


extern "C" pid_t waitpid(pid_t pid, int *status, int flags)
{
	extern int remote_desc;

	fd_set readset;

	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

PDBG("waitpid(%d, %d)", pid, flags);
	while(1) {
		
		FD_ZERO (&readset);

		if (remote_desc != -1)
			FD_SET (remote_desc, &readset);

		if (pid == -1) {

			FD_SET(_new_thread_pipe[0], &readset);

			Thread_capability thread_cap = csc->first();

			while (thread_cap.valid()) {
				PDBG("adding signal pipe fd of lwpid %lu to set", csc->lwpid(thread_cap));
				FD_SET(csc->signal_pipe_read_fd(thread_cap), &readset);
				thread_cap = csc->next(thread_cap);
			}

		} else {

			FD_SET(csc->signal_pipe_read_fd(csc->thread_cap(pid)), &readset);
		}

		struct timeval wnohang_timeout = {0, 0};
		struct timeval *timeout = (flags & WNOHANG) ? &wnohang_timeout : NULL;

//PDBG("calling select()");
		/* TODO: determine the highest fd in the set for optimization */
		int res = select(FD_SETSIZE, &readset, 0, 0, timeout);

		//if (debug_threads)
			//PDBG("select() returned %d", res);

		if (res > 0) {

			if ((remote_desc != -1) && FD_ISSET(remote_desc, &readset)) {

				/* received input from GDB */

				int cc;
				char c = 0;

				cc = read (remote_desc, &c, 1);

				if (cc == 1 && c == '\003' && current_inferior != NULL) {
					/* this causes a SIGINT to be delivered to one of the threads */
					(*the_target->request_interrupt)();
					continue;
				} else {
					if (verbose)
						PDBG("input_interrupt, count = %d c = %d ('%c')", cc, c, c);
				}

			} else if (FD_ISSET(_new_thread_pipe[0], &readset)) {

				unsigned long lwpid = GENODE_MAIN_LWPID;

				genode_stop_thread(lwpid);

				*status = W_STOPCODE(SIGTRAP) | (PTRACE_EVENT_CLONE << 16);

				return lwpid;

			} else {

				/* received a signal */

				Thread_capability thread_cap = csc->first();

				while (thread_cap.valid()) {
					if (FD_ISSET(csc->signal_pipe_read_fd(thread_cap), &readset))
						break;
					thread_cap = csc->next(thread_cap);
				}

				if (!thread_cap.valid())
					continue;

				int signal;
				read(csc->signal_pipe_read_fd(thread_cap), &signal, sizeof(signal));

				unsigned long lwpid = csc->lwpid(thread_cap);

				//if (debug_threads)
					PDBG("thread %lu received signal %d", lwpid, signal);

				if (signal == SIGTRAP) {

					sigtrap_lwpid = lwpid;
					
				} else if (signal == SIGSTOP) {

					/*
					 * Check if a SIGTRAP is pending
					 * 
					 * This can happen if a single-stepped thread gets paused while gdbserver
					 * handles a signal of a different thread and the exception signal after
					 * the single step has not arrived yet. In this case, the SIGTRAP must be
					 * delivered first, otherwise gdbserver would single-step the thread again.
					 */

					Thread_capability thread_cap = csc->thread_cap(lwpid);

					Thread_state thread_state = csc->state(thread_cap);

					if (thread_state.exception) {
						/* resend the SIGSTOP signal */
						csc->send_signal(thread_cap, SIGSTOP, 0);
						continue;
					}

				} else if (signal == SIGINFO) {

					if (verbose)
						PDBG("received SIGINFO for new lwpid %lu", lwpid);

					if (lwpid != GENODE_MAIN_LWPID)
						write(_new_thread_pipe[1], &lwpid, sizeof(lwpid));

					/*
					 * First signal of a new thread. On Genode originally a
					 * SIGTRAP, but gdbserver expects SIGSTOP.
					 */

					signal = SIGSTOP;
				}

				*status = W_STOPCODE(signal);

				return lwpid;
			}

		} else {

			return res;

		}
	}
}


extern "C" long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data)
{
	const char *request_str = 0;

	switch (request) {
		case PTRACE_TRACEME:    request_str = "PTRACE_TRACEME";    break;
		case PTRACE_PEEKTEXT:   request_str = "PTRACE_PEEKTEXT";   break;
		case PTRACE_PEEKUSER:   request_str = "PTRACE_PEEKUSER";   break;
		case PTRACE_POKETEXT:   request_str = "PTRACE_POKETEXT";   break;
		case PTRACE_POKEUSER:   request_str = "PTRACE_POKEUSER";   break;
		case PTRACE_CONT:       request_str = "PTRACE_CONT";       break;
		case PTRACE_KILL:       request_str = "PTRACE_KILL";       break;
		case PTRACE_SINGLESTEP: request_str = "PTRACE_SINGLESTEP"; break;
		case PTRACE_GETREGS:    request_str = "PTRACE_GETREGS";    break;
		case PTRACE_SETREGS:    request_str = "PTRACE_SETREGS";    break;
		case PTRACE_ATTACH:     request_str = "PTRACE_ATTACH";     break;
		case PTRACE_DETACH:     request_str = "PTRACE_DETACH";     break;
		case PTRACE_GETEVENTMSG:
			/*
			 * Only PTRACE_EVENT_CLONE is currently supported.
			 *
			 * Read the lwpid of the new thread from the pipe.
			 */
			read(_new_thread_pipe[0], data, sizeof(unsigned long));
			return 0;
		case PTRACE_GETREGSET: request_str = "PTRACE_GETREGSET";  break;
	}

	PWRN("ptrace(%s (0x%x)) called - not implemented!", request_str, request);

	errno = EINVAL;
	return -1;
}


extern "C" int fork()
{
	/* create the thread announcement pipe */

	if (pipe(_new_thread_pipe) != 0) {
		PERR("could not create the 'new thread' pipe");
		return -1;
	}

	/* extract target filename from config file */

	static char filename[32] = "";

	try {
		config()->xml_node().sub_node("target").attribute("name").value(filename, sizeof(filename));
	} catch (Xml_node::Nonexistent_sub_node) {
		PERR("Error: Missing '<target>' sub node.");
		return -1;
	} catch (Xml_node::Nonexistent_attribute) {
		PERR("Error: Missing 'name' attribute of '<target>' sub node.");
		return -1;
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
		return -1;
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
		return -1;
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

	_genode_child_resources = child->genode_child_resources();

	child->start();

	PDBG("main thread ready");

	return GENODE_MAIN_LWPID;
}


extern "C" int kill(pid_t pid, int sig)
{
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->thread_cap(pid);

	if (!thread_cap.valid()) {
		PERR("%s: could not find thread capability for lwpid %d",
		     __PRETTY_FUNCTION__, pid);
		return -1;
	}

	return csc->send_signal(thread_cap, sig, 0);
}


extern "C" int initial_breakpoint_handler(CORE_ADDR addr)
{
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();
	return csc->handle_initial_breakpoint(sigtrap_lwpid);
}


void genode_set_initial_breakpoint_at(CORE_ADDR addr)
{
	set_breakpoint_at(addr, initial_breakpoint_handler);
}


void genode_remove_thread(unsigned long lwpid)
{
	int pid = GENODE_MAIN_LWPID;
	linux_detach_one_lwp((struct inferior_list_entry *)
		find_thread_ptid(ptid_build(GENODE_MAIN_LWPID, lwpid, 0)), &pid);
}


extern "C" void genode_stop_all_threads()
{
PDBG("genode_stop_all_threads()");
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Lock::Guard stop_new_threads_lock_guard(csc->stop_new_threads_lock());

	csc->stop_new_threads(true);

	Thread_capability thread_cap = csc->first();

	while (thread_cap.valid()) {
		csc->pause(thread_cap);
		thread_cap = csc->next(thread_cap);
	}
}


void genode_resume_all_threads()
{
PDBG("genode_resume_all_threads()");
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

   	Lock::Guard stop_new_threads_guard(csc->stop_new_threads_lock());

	csc->stop_new_threads(false);

	Thread_capability thread_cap = csc->first();

	while (thread_cap.valid()) {
		csc->single_step(thread_cap, false);
		csc->resume(thread_cap);
		thread_cap = csc->next(thread_cap);
	}
}


int genode_detach(int pid)
{
    genode_resume_all_threads();

    return 0;
}


int genode_kill(int pid)
{
    /* TODO */
    if (verbose) PDBG("not implemented, just detaching instead...");

    return genode_detach(pid);
}


void genode_continue_thread(unsigned long lwpid, int single_step)
{
PDBG("genode_continue_thread(%lu, %d)", lwpid, single_step);
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->thread_cap(lwpid);

	if (!thread_cap.valid()) {
		PERR("%s: could not find thread capability for lwpid %lu",
		     __PRETTY_FUNCTION__, lwpid);
		return;
	}

	csc->single_step(thread_cap, single_step);
	csc->resume(thread_cap);
}


void genode_fetch_registers(struct regcache *regcache, int regno)
{
	unsigned long reg_content = 0;

	if (regno == -1) {
		for (regno = 0; regno < the_low_target.num_regs; regno++) {
			if (genode_fetch_register(regno, &reg_content) == 0)
				supply_register(regcache, regno, &reg_content);
			else
				supply_register(regcache, regno, 0);
		}
	} else {
		if (genode_fetch_register(regno, &reg_content) == 0)
			supply_register(regcache, regno, &reg_content);
		else
			supply_register(regcache, regno, 0);
	}
}


void genode_store_registers(struct regcache *regcache, int regno)
{
	if (debug_threads) PDBG("genode_store_registers(): regno = %d", regno);

	unsigned long reg_content = 0;

	if (regno == -1) {
		for (regno = 0; regno < the_low_target.num_regs; regno++) {
			if ((size_t)register_size(regno) <= sizeof(reg_content)) {
				collect_register(regcache, regno, &reg_content);
				genode_store_register(regno, reg_content);
			}
		}
	} else {
		if ((size_t)register_size(regno) <= sizeof(reg_content)) {
			collect_register(regcache, regno, &reg_content);
			genode_store_register(regno, reg_content);
		}
	}
}


class Memory_model
{
	private:

		Lock _lock;

		Rm_session_component * const _address_space;

		/**
		 * Representation of a currently mapped region
		 */
		struct Mapped_region
		{
			Rm_session_component::Region *_region;
			unsigned char                *_local_base;

			Mapped_region() : _region(0), _local_base(0) { }

			bool valid() { return _region != 0; }

			bool is_loaded(Rm_session_component::Region const * region)
			{
				return _region == region;
			}

			void flush()
			{
				if (!valid()) return;
				env()->rm_session()->detach(_local_base);
				_local_base = 0;
				_region = 0;
			}

			void load(Rm_session_component::Region *region)
			{
				if (region == _region)
					return;

				if (!region || valid())
					flush();

				if (!region)
					return;

				try {
					_region     = region;
					_local_base = env()->rm_session()->attach(_region->ds_cap(),
					                                          0, _region->offset());
				} catch (Rm_session::Attach_failed) {
					flush();
					PERR("Memory_model: RM attach failed");
				}
			}

			unsigned char *local_base() { return _local_base; }
		};

		enum { NUM_MAPPED_REGIONS = 1 };

		Mapped_region _mapped_region[NUM_MAPPED_REGIONS];

		unsigned _evict_idx;

		/**
		 * Return local address of mapped region
		 *
		 * The function returns 0 if the mapping fails
		 */
		unsigned char *_update_curr_region(Rm_session_component::Region *region)
		{
			for (unsigned i = 0; i < NUM_MAPPED_REGIONS; i++) {
				if (_mapped_region[i].is_loaded(region))
					return _mapped_region[i].local_base();
			}

			/* flush one currently mapped region */
			_evict_idx++;
			if (_evict_idx == NUM_MAPPED_REGIONS)
				_evict_idx = 0;

			_mapped_region[_evict_idx].load(region);

			return _mapped_region[_evict_idx].local_base();
		}

	public:

		Memory_model(Rm_session_component *address_space)
		:
			_address_space(address_space), _evict_idx(0)
		{ }

		unsigned char read(void *addr)
		{
			Lock::Guard guard(_lock);

			addr_t offset_in_region = 0;

			Rm_session_component::Region *region =
				_address_space->find_region(addr, &offset_in_region);

			unsigned char *local_base = _update_curr_region(region);

			if (!local_base) {
				PWRN("Memory model: no memory at address %p", addr);
				throw No_memory_at_address();
			}

			unsigned char value =
				local_base[offset_in_region];

			if (verbose)
				Genode::printf("read addr=%p, value=%x\n", addr, value);

			return value;
		}

		void write(void *addr, unsigned char value)
		{
			if (verbose)
				Genode::printf("write addr=%p, value=%x\n", addr, value);

			Lock::Guard guard(_lock);

			addr_t offset_in_region = 0;
			Rm_session_component::Region *region =
				_address_space->find_region(addr, &offset_in_region);

			unsigned char *local_base = _update_curr_region(region);

			if (!local_base) {
				PWRN("Memory model: no memory at address %p", addr);
				PWRN("(attempted to write %x)", (int)value);
				throw No_memory_at_address();
			}

			local_base[offset_in_region] = value;
		}
};


/**
 * Return singleton instance of memory model
 */
static Memory_model *memory_model()
{
	static Memory_model inst(genode_child_resources()->rm_session_component());
	return &inst;
}


unsigned char genode_read_memory_byte(void *addr)
{
	return memory_model()->read(addr);
}


int genode_read_memory(CORE_ADDR memaddr, unsigned char *myaddr, int len)
{
	if (debug_threads)
		PDBG("genode_read_memory(%llx, %p, %d)", memaddr, myaddr, len);

	if (myaddr)
		try {
			for (int i = 0; i < len; i++)
				myaddr[i] = genode_read_memory_byte((void*)((unsigned long)memaddr + i));
		} catch (No_memory_at_address) {
			return EFAULT;
		}
		
  return 0;
}


void genode_write_memory_byte(void *addr, unsigned char value)
{
	memory_model()->write(addr, value);
}


int genode_write_memory (CORE_ADDR memaddr, const unsigned char *myaddr, int len)
{
	if (debug_threads)
		PDBG("genode_write_memory(%llx, %p, %d)", memaddr, myaddr, len);

	if (myaddr && (len > 0)) {
		if (debug_threads) {
			/* Dump up to four bytes.  */
			unsigned int val = * (unsigned int *) myaddr;
			if (len == 1)
				val = val & 0xff;
			else if (len == 2)
				val = val & 0xffff;
			else if (len == 3)
				val = val & 0xffffff;
			fprintf(stderr, "Writing %0*x to 0x%08lx", 2 * ((len < 4) ? len : 4),
			        val, (long)memaddr);
		}

	for (int i = 0; i < len; i++)
		try {
			genode_write_memory_byte((void*)((unsigned long)memaddr + i), myaddr[i]);
		} catch (No_memory_at_address) {
			return EFAULT;
		}
	}

	return 0;
}
