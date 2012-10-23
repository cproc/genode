/*
 * \brief  Board definitions for the i.MX31
 * \author Martin Stein
 * \author Norman Feske
 * \date   2011-11-03
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__PLATFORM__BOARD_H_
#define _INCLUDE__PLATFORM__BOARD_H_

/* Genode includes */
#include <util/mmio.h>

namespace Genode
{
	/**
	 * i.MX31 motherboard
	 */
	struct Board
	{
		enum {
			CSD0_SDRAM_BASE = 0x80000000,
			CSD0_SDRAM_SIZE = 0x10000000,

			UART_1_IRQ       = 45,
			UART_1_MMIO_BASE = 0x43f90000,
			UART_1_MMIO_SIZE = 0x00004000,

			EPIT_1_IRQ       = 28,
			EPIT_1_MMIO_BASE = 0x53f94000,
			EPIT_1_MMIO_SIZE = 0x00004000,

			AVIC_MMIO_BASE = 0x68000000,
			AVIC_MMIO_SIZE = 0x04000000,

			AIPS_1_MMIO_BASE = 0x43F00000,
			AIPS_1_MMIO_SIZE = 0x00004000,

			AIPS_2_MMIO_BASE = 0x53F00000,
			AIPS_2_MMIO_SIZE = 0x00004000,
		};
	};
}

#endif /* _INCLUDE__PLATFORM__BOARD_H_ */

