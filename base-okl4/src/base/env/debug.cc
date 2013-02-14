/*
 * \brief  OKL4-specific debug utilities
 * \author Christian Prochaska
 * \date   2012-02-14
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/*
 * With the enabled 'DEBUG' flag, status information can be printed directly
 * via a OKL4 system call by using the 'raw_write_str' function. This output
 * bypasses the Genode 'LOG' mechanism, which is useful for debugging low-level
 * code such as a libC back-end.
 */
#define DEBUG 1


#if DEBUG
namespace Okl4 {
#include <l4/kdebug.h>
}
#endif /* DEBUG */


/**
 * Write function targeting directly the OKL4 system call layer and bypassing
 * any Genode code.
 */
extern "C" int raw_write_str(const char *str)
{
#if DEBUG
	unsigned len = 0;
	for (; str[len] != 0; len++)
		Okl4::L4_KDB_PrintChar(str[len]);
	return len;
#endif /* DEBUG */
}
