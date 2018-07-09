/*
 * \brief  LibC test program used during the libC porting process
 * \author Norman Feske
 * \author Alexander Böttcher
 * \author Christian Helmuth
 * \date   2008-10-22
 */

/*
 * Copyright (C) 2008-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/*
 * Mixing Genode headers and libC to see it they collide...
 */

/* Genode includes */
#include <base/env.h>

/* libC includes */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>


int main(int argc, char **argv)
{
	printf("--- libC test ---\n");

	for (;;) {
		int fd = open("/dev/zero", O_RDONLY);
		Genode::log("fd: ", fd);
	}

	return 0;
}
