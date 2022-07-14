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

/* libC includes */
extern "C" {
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/limits.h>
#include <sys/random.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
}

int main(int argc, char **argv)
{
	{
		unsigned long long buf = 0;
		getrandom(&buf, sizeof(buf), 0);
		printf("getrandom %llx\n", buf);
	}

	{
		unsigned long long buf = 0;
		getentropy(&buf, sizeof(buf));
		printf("getentropy %llx\n", buf);
	}

	return 0;
}
