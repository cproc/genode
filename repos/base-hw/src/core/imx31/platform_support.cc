/*
 * \brief   Platform implementations specific for base-hw and i.MX31
 * \author  Norman Feske
 * \date    2012-08-30
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <platform.h>
#include <board.h>
#include <pic.h>
#include <cpu.h>
#include <kernel/irq.h>

using namespace Genode;

namespace Kernel { void init_platform(); }

/**
 * Interrupts that core shall provide to users
 */
static unsigned irq_ids[] =
{
	Board::EPIT_2_IRQ
};

enum { IRQ_IDS_SIZE = sizeof(irq_ids)/sizeof(irq_ids[0]) };


void Kernel::init_platform()
{
	/* make user IRQs become known by cores IRQ session backend and kernel */
	static uint8_t _irqs[IRQ_IDS_SIZE][sizeof(Irq)];
	for (unsigned i = 0; i < IRQ_IDS_SIZE; i++) {
		new (_irqs[i]) Irq(irq_ids[i]);
	}
}


unsigned * Platform::_irq(unsigned const i)
{
	return i < IRQ_IDS_SIZE ? &irq_ids[i] : 0;
}


Native_region * Platform::_ram_regions(unsigned const i)
{
	static Native_region _regions[] =
	{
		{ Board::RAM_0_BASE, Board::RAM_0_SIZE }
	};
	return i < sizeof(_regions)/sizeof(_regions[0]) ? &_regions[i] : 0;
}


Native_region * Platform::_mmio_regions(unsigned const i)
{
	static Native_region _regions[] =
	{
		/*
		 * The address range below 0x30000000 is used for secure ROM, ROM, and
		 * internal RAM.
		 */
		{ 0x30000000, 0x50000000 },
		/*
		 * The address range between 0x8000000 and 0x9fffffff is designated for
		 * SDRAM. The remaining address range is populated with peripherals.
		 */
		{ 0xa0000000, 0x24000000 }
	};
	return i < sizeof(_regions)/sizeof(_regions[0]) ? &_regions[i] : 0;
}


Native_region * Platform::_core_only_mmio_regions(unsigned const i)
{
	static Native_region _regions[] =
	{
		/* core UART */
		{ Board::UART_1_MMIO_BASE, Board::UART_1_MMIO_SIZE },

		/* core timer */
		{ Board::EPIT_1_MMIO_BASE, Board::EPIT_1_MMIO_SIZE },

		/* interrupt controller */
		{ Board::AVIC_MMIO_BASE, Board::AVIC_MMIO_SIZE },

		/* bus interface controller */
		{ Board::AIPS_1_MMIO_BASE, Board::AIPS_1_MMIO_SIZE },
		{ Board::AIPS_2_MMIO_BASE, Board::AIPS_2_MMIO_SIZE },
	};
	return i < sizeof(_regions)/sizeof(_regions[0]) ? &_regions[i] : 0;
}


Cpu::User_context::User_context() { cpsr = Psr::init_user(); }
