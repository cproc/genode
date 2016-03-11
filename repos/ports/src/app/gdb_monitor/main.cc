/*
 * \brief  GDB Monitor
 * \author Christian Prochaska
 * \date   2010-09-16
 */

/*
 * Copyright (C) 2010-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>
#include <base/process.h>

#include "rom.h"

using namespace Genode;
using namespace Gdb_monitor;

/*
 * Suppress messages of libc dummy functions
 */
extern "C" int _sigaction()   { return -1; }
extern "C" int  getpid()      { return -1; }
extern "C" int  sigprocmask() { return -1; }
extern "C" int _sigprocmask() { return -1; }

/*
 * version.c
 */
extern "C" const char version[] = "7.3.1";
extern "C" const char host_name[] = "";


extern "C" int gdbserver_main(int argc, const char *argv[]);

int main()
{
	/* look for dynamic linker */
	try {
		Rom_connection ldso_rom("ld.lib.so");
		Process::dynamic_linker(clone_rom(ldso_rom.dataspace()));
	} catch (...) {
		PDBG("ld.lib.so not found");
	}

	int argc = 4;
	const char *argv[] = { "gdbserver", "/dev/terminal", "target", 0 };

	return gdbserver_main(argc, argv);
}
