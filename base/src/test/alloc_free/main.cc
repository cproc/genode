/*
 * \brief  Test for allocating and freeing of RAM dataspaces
 * \author Christian Prochaska
 * \date   2013-04-10
 *
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/env.h>
#include <base/printf.h>

namespace Fiasco {
#include <l4/sys/kdebug.h>
}

using namespace Genode;

int main(int argc, char **argv)
{
	Ram_dataspace_capability cap;

	PDBG("Test started");

	for (int i = 0; i < 10000; i++) {
		//PDBG("calling alloc()");
		cap = env()->ram_session()->alloc(4096);
		//PDBG("calling free()");
		env()->ram_session()->free(cap);
	}

	PDBG("Test finished");

	enter_kdebug("finished");

	return 0;
}
