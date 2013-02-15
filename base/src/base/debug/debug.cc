/*
 * \brief  Debug utilities
 * \author Christian Prochaska
 * \date   2012-02-15
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#define DEBUG 1

#if DEBUG
#include <base/debug.h>
#include <core_console.h>
using namespace Genode;
#endif /* DEBUG */


void raw_printf(const char *format, ...)
{
#if DEBUG
	static Core_console core_console;
	va_list list;
	va_start(list, format);
	core_console.vprintf(format, list);
	va_end(list);
#endif /* DEBUG */
}
