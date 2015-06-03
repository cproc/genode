/*
 * \brief  Memory region types
 * \author Norman Feske
 * \author Adrian-Ken Rueegsegger
 * \author Reto Buerki
 * \date   2015-06-23
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#ifndef _MEMORY_H_
#define _MEMORY_H_

/* Genode includes */
#include <base/env.h>
#include <base/lock.h>
#include <util/list.h>
#include <os/attached_io_mem_dataspace.h>

/* VirtualBox includes */
#include <VBox/vmm/pgm.h>
#include <VBox/vmm/pdmdev.h>

struct Mem_region;

struct Mem_region : Genode::List<Mem_region>::Element,
					Genode::Attached_io_mem_dataspace
{
	typedef Genode::Ram_session Ram_session;
	typedef Genode::size_t      size_t;
	typedef Genode::addr_t      addr_t;

	PPDMDEVINS           pDevIns;
	unsigned const       iRegion;
	RTGCPHYS             vm_phys;
	PFNPGMR3PHYSHANDLER  pfnHandlerR3;
	void                *pvUserR3;
	PGMPHYSHANDLERTYPE   enmType;
	size_t				 region_size;

	addr_t _phys_base()
	{
		struct Region_info
		{
			addr_t base;
			size_t size;
		};
		static unsigned counter;
		static Region_info regions[] = {
			/* RAM phys 0x0 */
			{ 0x800000000, 0x10000000 },
			/* pcbios phys 0xe1000 */
			{ 0x810000000, 0x1000 },
			/* pcbios phys 0xf0000*/
			{ 0x820000000, 0x10000 },
			/* pcbios 0xffff0000 */
			{ 0x830000000, 0x10000 },
			/* VMMDev */
			{ 0x840000000, 0x400000 },
			/* VMMDev */
			{ 0x850000000, 0x4000 },
			/* vga */
			{ 0x860000000, 0xc00000 },
			/* vga phys 0xc0000 */
			{ 0x870000000, 0x9000 },
			/* acpi phys e0000 */
			{ 0x880000000, 0x1000 },
			{ 0x0, 0x0 },
		};

		Region_info cur_region = regions[counter++];

		if (cur_region.size == 0)
		{
			PERR("Region size is zero!!!");
			return 0;
		}
		return cur_region.base;
	}

	size_t size() const { return region_size; }

	Mem_region(Ram_session &ram, size_t size, PPDMDEVINS pDevIns,
	           unsigned iRegion)
	:
		Attached_io_mem_dataspace(_phys_base(), size),
		pDevIns(pDevIns),
		iRegion(iRegion),
		vm_phys(0), pfnHandlerR3(0), pvUserR3(0),
		region_size(size)
	{
		PDBG("Virt address=0x%p, size 0x%zx, dev=%s ",
				local_addr<char>(), size,
               pDevIns && pDevIns->pReg ? pDevIns->pReg->szName : 0);
	}
};

#endif /* _MEMORY_H_ */
