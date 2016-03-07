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


static Lock &main_thread_ready_lock()
{
	static Lock _main_thread_ready_lock(Lock::LOCKED);
	return _main_thread_ready_lock;
}


Genode_child_resources *genode_child_resources()
{
	return (Genode_child_resources*)(current_process()->_private->genode_child_resources);
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
#if 0
	int pid = GENODE_LWP_BASE;
	linux_detach_one_lwp((struct inferior_list_entry *)
		find_thread_ptid(ptid_build(GENODE_LWP_BASE, lwpid, 0)), &pid);
#endif
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
