/*
 * \brief   Platform implementations specific for x86
 * \author  Norman Feske
 * \author  Reto Buerki
 * \date    2013-04-05
 */

/*
 * Copyright (C) 2013-2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <platform.h>
#include <board.h>
#include <cpu.h>

#include <multiboot.h>

extern "C" Genode::addr_t __initial_bx;

using namespace Genode;

Native_region * Platform::_ram_regions(unsigned const i)
{
	static Native_region _regions[16];

	Genode::Multiboot_info amb(__initial_bx);

	Multiboot_info::Mmap v = amb.phys_ram(i);
	if (!v.base)
		return nullptr;

	Genode::uint64_t base = v.read<Multiboot_info::Mmap::Addr>();
	Genode::uint64_t size = v.read<Multiboot_info::Mmap::Length>();

	if (i >= sizeof(_regions) / sizeof(_regions[0])) {
		PWRN("physical ram region 0x%llx+0x%llx will be not used", base, size);
		return nullptr;
	}

	if (!_regions[i].size) {
		if (base == 0 && size >= 4096) {
			/*
			 * Exclude first physical page, so that it will become part of the
			 * MMIO allocator. The framebuffer requests this page as MMIO.
			 */
			base  = 4096;
			size -= 4096;
		}
		_regions[i] = { base, size };
	}

	return &_regions[i];
}


void Platform::_init_io_port_alloc()
{
	_io_port_alloc.add_range(0, 0x10000);
}


/**
 * Remove given exclude memory regions from specified allocator.
 */
static void alloc_exclude_regions(Range_allocator * const alloc,
                                  Region_pool excl_regions)
{
	Native_region * r = excl_regions(0);
	for (unsigned i = 0; r; r = excl_regions(++i))
		alloc->remove_range(r->base, r->size);
}


void Platform::_init_io_mem_alloc()
{
	/* add entire adress space minus the RAM memory regions */
	_io_mem_alloc.add_range(0, ~0x0UL);
	alloc_exclude_regions(&_io_mem_alloc, _ram_regions);
	alloc_exclude_regions(&_io_mem_alloc, _core_only_ram_regions);
	alloc_exclude_regions(&_io_mem_alloc, _core_only_mmio_regions);

	/* exclude all mmio regions from virt allocator of core */
	alloc_exclude_regions(_core_mem_alloc.virt_alloc(), _core_only_mmio_regions);
}


long Platform::irq(long const user_irq)
{
	/* remap IRQ requests to fit I/O APIC configuration */
	if (user_irq) return user_irq + Board::VECTOR_REMAP_BASE;
	return Board::TIMER_VECTOR_USER;
}
