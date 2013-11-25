/*
 * \brief  Test for unmapping of a sub RM session
 * \author Christian Prochaska
 * \date   2013-11-25
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <ram_session/ram_session.h>
#include <rm_session/connection.h>

using namespace Genode;

static const bool attach_sub_rm_session = true;
static const bool detach_sub_ds = false;

int main(int, char **)
{
	static Rm_connection rm(0, 4096);


	/* create two dataspaces and fill them with different values */

	Ram_dataspace_capability ds1 = env()->ram_session()->alloc(4096);
	unsigned char *ds1_addr = env()->rm_session()->attach(ds1);
	memset(ds1_addr, 1, 4096);

	Ram_dataspace_capability ds2 = env()->ram_session()->alloc(4096);
	unsigned char *ds2_addr = env()->rm_session()->attach(ds2);	
	memset(ds2_addr, 2, 4096);

	if (attach_sub_rm_session) {

		/* attach the sub RM session */
		unsigned char *rm_addr = env()->rm_session()->attach(rm.dataspace());

		PDBG("rm_addr = 0x%p", rm_addr);

		/* attach ds1 to the sub RM session */
		unsigned char *new_ds1_addr = rm.attach(ds1);

		/* read out first value of attached ds1, should be 1 */
		PDBG("*0x%p = %x", rm_addr, *rm_addr);

		if (detach_sub_ds) {
			/* detach ds1 from the sub RM session */
			rm.detach(new_ds1_addr);
		}

		/* detach the sub RM session */
		env()->rm_session()->detach(rm_addr);
	}

	/* attach ds2 to the main RM session */
	unsigned char *new_ds2_addr = env()->rm_session()->attach(ds2);

	PDBG("new_ds2_addr = 0x%p", new_ds2_addr);

	/* read out first value of attached ds2, should be 2 */
	PDBG("*0x%p = %x", new_ds2_addr, *new_ds2_addr);

	return 0;
}

