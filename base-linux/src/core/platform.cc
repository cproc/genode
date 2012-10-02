/*
 * \brief   Linux platform interface implementation
 * \author  Norman Feske
 * \date    2006-06-13
 */

/*
 * Copyright (C) 2006-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/lock.h>
#include <base/service.h>

/* local includes */
#include "platform.h"
#include "core_env.h"
#include <io_port_root.h>

/* Linux includes */
#include <linux_syscalls.h>
#include <linux_rpath.h>


using namespace Genode;


static char _some_mem[80*1024*1024];
static Lock _wait_for_exit_lock(Lock::LOCKED);  /* exit() sync */


static void signal_handler(int signum)
{
	_wait_for_exit_lock.unlock();
}


Platform::Platform()
: _ram_alloc(0)
{
	/* catch control-c */
	lx_sigaction(2, signal_handler);

	/* create resource directory under /tmp */
	lx_mkdir(lx_rpath(), S_IRWXU);

	_ram_alloc.add_range((addr_t)_some_mem, sizeof(_some_mem));
}


void Platform::wait_for_exit()
{
	/* block until exit condition is satisfied */
	try { _wait_for_exit_lock.lock(); }
	catch (Blocking_canceled) { };
}


void Platform::add_local_services(Rpc_entrypoint *e, Sliced_heap *sliced_heap,
                                  Core_env *env, Service_registry *local_services)
{
	/* add x86 specific ioport service */
	static Io_port_root io_port_root(env->cap_session(), io_port_alloc(), sliced_heap);
	static Local_service io_port_ls(Io_port_session::service_name(), &io_port_root);
	local_services->insert(&io_port_ls);
}


void Core_parent::exit(int exit_value)
{
	lx_exit_group(exit_value);
}
