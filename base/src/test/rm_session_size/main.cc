/*
 * \brief  Testing Rm_session size effects
 * \author Christian Prochaska
 * \date   2013-07-10
 *
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>
#include <dataspace/client.h>
#include <rm_session/connection.h>

using namespace Genode;

int main(int argc, char **argv)
{
	/* a size not of page granularity */
	enum {SIZE = 4096 + 1 };

	Rm_connection rm(0, SIZE);

	Dataspace_capability rm_ds = rm.dataspace();
	printf("rm ds size  = %zu\n", Dataspace_client(rm_ds).size());

	Ram_dataspace_capability ram_ds = env()->ram_session()->alloc(SIZE);
	printf("ram ds size = %zu\n", Dataspace_client(ram_ds).size());

	rm.attach(ram_ds, SIZE);

	printf("Test succeeded\n");

	return 0;
}
