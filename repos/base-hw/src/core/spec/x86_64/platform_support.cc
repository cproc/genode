/*
 * \brief   Platform implementations specific for x86_64
 * \author  Reto Buerki
 * \date    2015-05-04
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <platform.h>
#include <board.h>
#include <pic.h>
#include <kernel/kernel.h>

using namespace Genode;

/* contains physical pointer to multiboot */
extern "C" Genode::addr_t __initial_bx;


Native_region * Platform::_core_only_mmio_regions(unsigned const i)
{
	static Native_region _regions[] =
	{
		{ Board::MMIO_LAPIC_BASE,  Board::MMIO_LAPIC_SIZE  },
		{ Board::MMIO_IOAPIC_BASE, Board::MMIO_IOAPIC_SIZE },
		{ 0, 0}
	};

	unsigned const max = sizeof(_regions) / sizeof(_regions[0]);

	if (!_regions[max - 1].size)
		_regions[max - 1] = { __initial_bx & ~0xFFFUL, get_page_size() };

	return i < max ? &_regions[i] : nullptr;
}


void Platform::setup_irq_mode(unsigned irq_number, unsigned trigger,
                              unsigned polarity)
{
	Kernel::pic()->ioapic.setup_irq_mode(irq_number, trigger, polarity);
}
