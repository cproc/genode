/*
 * \brief  Console backend for seL4
 * \author Norman Feske
 * \date   2014-10-14
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/console.h>

/* seL4 includes */
#include <sel4/arch/functions.h>
#include <sel4/arch/syscalls.h>

namespace Genode
{
	class Core_console : public Console
	{
		protected:

			void _out_char(char c)
			{
				seL4_DebugPutChar(c);
			}
	};
}

