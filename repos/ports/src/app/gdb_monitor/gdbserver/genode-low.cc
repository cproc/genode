/*
 * \brief  Genode backend for GDBServer (C++)
 * \author Christian Prochaska
 * \author Norman Feske
 * \date   2011-05-06
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <signal.h>
#include <sys/wait.h>
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

static bool verbose = false;

static int _new_thread_pipe[2];

using namespace Genode;
using namespace Gdb_monitor;


extern "C" void *add_lwp(ptid_t ptid);


static Lock &main_thread_ready_lock()
{
	static Lock _main_thread_ready_lock(Lock::LOCKED);
	return _main_thread_ready_lock;
}


Genode_child_resources *genode_child_resources()
{
	return (Genode_child_resources*)(current_process()->_private->genode_child_resources);
}


static int
add_signal_pipe_read_fd_to_set(struct inferior_list_entry *entry, void *args)
{
	fd_set *readset = (fd_set*)args;

	FD_SET(genode_thread_signal_pipe_read_fd(ptid_get_lwp(entry->id)), readset);

	return 0;
}


static int
signal_pipe_read_fd_in_set(struct inferior_list_entry *entry, void *args)
{
	fd_set *readset = (fd_set*)args;

	return FD_ISSET(genode_thread_signal_pipe_read_fd(ptid_get_lwp(entry->id)), readset);
}


extern "C" pid_t waitpid(pid_t pid, int *status, int flags)
{
	extern int remote_desc;

	fd_set readset;

	while(1) {

		FD_ZERO (&readset);

		if (remote_desc != -1)
			FD_SET (remote_desc, &readset);

		if (pid == -1) {
			FD_SET(genode_new_thread_pipe_read_fd(), &readset);
			find_inferior(&all_threads, add_signal_pipe_read_fd_to_set, &readset);
		} else
			FD_SET(genode_thread_signal_pipe_read_fd(pid), &readset);

		struct timeval wnohang_timeout = {0, 0};
		struct timeval *timeout = (flags & WNOHANG) ? &wnohang_timeout : NULL;


		/* TODO: determine the highest fd in the set for optimization */
		if (select(FD_SETSIZE, &readset, 0, 0, timeout) > 0) {

			if (debug_threads)
				fprintf(stderr, "select() returned\n");

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
					fprintf (stderr, "input_interrupt, count = %d c = %d ('%c')\n",
					         cc, c, c);
				}

			} else if (FD_ISSET(genode_new_thread_pipe_read_fd(), &readset)) {

				/* read the lwpid of the new thread from the pipe */
				unsigned long new_lwpid;
				read(genode_new_thread_pipe_read_fd(), &new_lwpid, sizeof(new_lwpid));
				
				/* make the new thread known to gdbserver */
				{
					ptid_t ptid = ptid_build (1, new_lwpid, 0);
					struct lwp_info *new_lwp = (struct lwp_info *) add_lwp (ptid);
					add_thread (ptid, new_lwp);
				}

				/* consume the SIGTRAP signal caused by initial single-stepping */
				{
					int signal;
					read(genode_thread_signal_pipe_read_fd(new_lwpid), &signal, sizeof(signal));
					fprintf(stderr, "new thread received signal %d\n", signal);
					if (signal != SIGTRAP)
						fprintf(stderr, "Error: first signal received by the new thread was not SIGTRAP\n");
				}

				/* resume the new thread */
				genode_continue_thread(new_lwpid, 0);

				continue;

			} else {

				/* received a signal */

				/* find a thread whose signal pipe is ready for reading */
				struct thread_info *thread = (struct thread_info*)
					find_inferior(&all_threads, signal_pipe_read_fd_in_set, &readset);

				if (!thread)
					continue;

				unsigned long lwpid = ptid_get_lwp(thread->entry.id);
				int signal;
				read(genode_thread_signal_pipe_read_fd(lwpid), &signal, sizeof(signal));

				if (debug_threads)
					fprintf(stderr, "thread %lu received signal %d\n", lwpid, signal);

				if (signal == SIGSTOP) {

					/*
					 * Check if a SIGTRAP is pending
					 * 
					 * This can happen if a single-stepped thread gets paused while gdbserver
					 * handles a signal of a different thread and the exception signal after
					 * the single step has not arrived yet. In this case, the SIGTRAP must be
					 * delivered first, otherwise gdbserver would single-step the thread again.
					 */

					Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

					Thread_capability thread_cap = csc->thread_cap(lwpid);

					Thread_state thread_state = csc->state(thread_cap);

					if (thread_state.exception) {
						/* resend the SIGSTOP signal */
						csc->send_signal(thread_cap, SIGSTOP, 0);
						continue;
					}
				}

				*status = _WSTOPPED | (signal << 8);

				return lwpid;
			}
		}
	}
}


void *genode_start_inferior()
{
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

	return child->genode_child_resources();
}


int genode_new_thread_pipe_read_fd()
{
	return _new_thread_pipe[0];
}


void genode_add_thread(unsigned long lwpid)
{
	if (lwpid == GENODE_LWP_BASE) {

		if (pipe(_new_thread_pipe) != 0)
			PERR("could not create the 'new thread' pipe");

		main_thread_ready_lock().unlock();

	} else

		write(_new_thread_pipe[1], &lwpid, sizeof(lwpid));
}


void genode_remove_thread(unsigned long lwpid)
{
	int pid = GENODE_LWP_BASE;
	linux_detach_one_lwp((struct inferior_list_entry *)
		find_thread_ptid(ptid_build(GENODE_LWP_BASE, lwpid, 0)), &pid);
}


void genode_wait_for_target_main_thread()
{
	/* wait until the target's main thread has been created */
	main_thread_ready_lock().lock();
}


#if 0
extern "C" void genode_detect_all_threads()
{
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->next(csc->first()); /* second thread */

	while (thread_cap.valid()) {
		linux_attach_lwp(csc->lwpid(thread_cap));
		thread_cap = csc->next(thread_cap);
	}
}
#endif


/*
 * Return 1 if the list entry has the lwpid given in 'args'.
 */
static int has_lwpid(struct inferior_list_entry *entry, void *args)
{
	unsigned long *lwpid = (unsigned long*)args;

	return ((unsigned long)ptid_get_lwp(entry->id) == *lwpid);
}

#if 0
extern "C" unsigned long genode_find_lwpid_of_new_thread()
{
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->first();

	while (thread_cap.valid()) {
		unsigned long lwpid = csc->lwpid(thread_cap);
		if (!find_inferior(&all_threads, has_lwpid, &lwpid)) {
			PDBG("new thread has lwpid %lu", lwpid);
			return lwpid;
		}
		thread_cap = csc->next(thread_cap);
	}

	PERR("could not find out the lwpid of the new thread");

	return 0;
}
#endif

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


extern "C" void genode_resume_all_threads()
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
    if (verbose) PDBG("genode_kill() called - not implemented\n");

    return genode_detach(pid);
}


void genode_stop_thread(unsigned long lwpid)
{
PDBG("genode_stop_thread(%lu)", lwpid);
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->thread_cap(lwpid);

	if (!thread_cap.valid()) {
		PERR("could not find thread capability for lwpid %lu", lwpid);
		return;
	}

	csc->pause(thread_cap);
}


void genode_continue_thread(unsigned long lwpid, int single_step)
{
PDBG("genode_continue_thread(%lu, %d)", lwpid, single_step);
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->thread_cap(lwpid);

	if (!thread_cap.valid()) {
		PERR("could not find thread capability for lwpid %lu", lwpid);
		return;
	}

	csc->single_step(thread_cap, single_step);
	csc->resume(thread_cap);
}


int genode_thread_signal_pipe_read_fd(unsigned long lwpid)
{
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->thread_cap(lwpid);

	if (!thread_cap.valid()) {
		PERR("could not find thread capability for lwpid %lu", lwpid);
		return -1;
	}

	return csc->signal_pipe_read_fd(thread_cap);
}


int genode_send_signal_to_thread(unsigned long lwpid, int signo, unsigned long *payload)
{
	Cpu_session_component *csc = genode_child_resources()->cpu_session_component();

	Thread_capability thread_cap = csc->thread_cap(lwpid);

	if (!thread_cap.valid()) {
		PERR("could not find thread capability for lwpid %lu", lwpid);
		return -1;
	}

	return csc->send_signal(thread_cap, signo, payload);
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
				return 0;
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
				return;
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


void genode_write_memory_byte(void *addr, unsigned char value)
{
	return memory_model()->write(addr, value);
}
