/*
 * \brief  Fiasco-specific part of RM-session implementation
 * \author Stefan Kalkowski
 * \date   2011-01-18
 */

/*
 * Copyright (C) 2011-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <rm_session_component.h>
#include <map_local.h>

/* Fiasco.OC includes */
namespace Fiasco {
#include <l4/sys/cache.h>
#include <l4/sys/task.h>
}

using namespace Genode;

void Rm_client::unmap(addr_t core_local_base, addr_t virt_base, size_t size)
{
	// TODO unmap it only from target space
	//unmap_local(core_local_base, size >> get_page_size_log2());

	//size_t size = num_pages << get_page_size_log2();
	addr_t addr = virt_base;

	/*
	 * XXX divide operation into flexpages greater than page size
	 */
	for (; addr < virt_base + size; addr += L4_PAGESIZE)
		l4_task_unmap(_platform_thread->platform_pd()->native_task().dst(),
		              l4_fpage(addr, L4_LOG2_PAGESIZE, Fiasco::L4_FPAGE_RW),
		              Fiasco::L4_FP_ALL_SPACES);

	Fiasco::l4_cache_clean_data(virt_base, virt_base + size);
}
