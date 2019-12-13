/*
 * \brief   Platform specific services for Rpi2
 * \author  Tomasz Gajewski
 * \date    2019-01-30
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/service.h>
#include <base/heap.h>

/* core includes */
#include <platform.h>
#include <platform_services.h>
#include <core_env.h>
#include <core_service.h>
#include <map_local.h>
//#include <vm_root.h>
#include <platform.h>

extern Genode::addr_t hypervisor_exception_vector;

/*
 * Add ARM virtualization specific vm service
 */
void Genode::platform_add_local_services(Rpc_entrypoint    &,
                                         Sliced_heap       &,
                                         Registry<Service> &,
                                         Trace::Source_registry &)
{
	using namespace Genode;

	// map_local(Platform::core_phys_addr((addr_t)&hypervisor_exception_vector),
	//           Hw::Mm::hypervisor_exception_vector().base, 1,
	//           Hw::PAGE_FLAGS_KERN_TEXT);

	// static Vm_root vm_root(ep, sh, core_env().ram_allocator(),
	//                        core_env().local_rm(), trace_sources);
	// static Core_service<Vm_session_component> vm_service(services, vm_root);
}
