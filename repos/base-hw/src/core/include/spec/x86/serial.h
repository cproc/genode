/*
 * \brief  Serial output driver for core
 * \author Stefan Kalkowski
 * \date   2012-10-24
 */

/*
 * Copyright (C) 2012-2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#pragma once

/* Genode includes */
#include <drivers/uart/x86_uart_base.h>

namespace Genode { class Serial; }

/**
 * Serial output driver for core
 */
class Genode::Serial : public X86_uart_base
{
	private:

		enum { IO_PORT = 0x3f8, CLOCK_UNUSED = 0 };

	public:

		Serial(unsigned baud_rate)
		: X86_uart_base(IO_PORT, CLOCK_UNUSED, baud_rate) { }
};
