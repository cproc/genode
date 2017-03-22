/*
 * \brief  LibC test program used during the libC porting process
 * \author Norman Feske
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
#include <stdio.h>
#include <stdlib.h>

void *addr = 0;

int main(int argc, char **argv)
{
	unsigned error_count = 0;

	printf("--- libC test ---\n");

	printf("Does printf work?\n");
	printf("We can find out by printing a floating-point number: %f. How does that work?\n", 1.2345);

	printf("Malloc test\n");
	addr = malloc(1234);
	printf("Malloc returned addr = %p\n", addr);

	/* check for 16 byte alignment of small sizes */
	for (unsigned size = 1; size < 67; size++) {
		for (unsigned j = 0; j < 32; j++) {
			addr = malloc(size);

			if (!((unsigned long)addr & 0xf))
				continue;

			printf("%u. malloc(%u) returned addr = %p - ERROR\n", j, size, addr);
			error_count ++;
		}
	}

	/* check for 16 byte alignment of larger sizes */
	unsigned size = 2048 - 15;
	for (unsigned j = 0; j < 4; j++) {
		addr = malloc(size);
		if (!((unsigned long)addr & 0xf))
			continue;

		printf("%u. malloc(%u) returned addr = %p - ERROR\n", j, size, addr);
		error_count ++;
	}

	for (unsigned size = 1 * 0x1000; size < 32 * 0x1000; size += 0x1000) {
		for (unsigned j = 0; j < 8; j++) {
			addr = malloc(size);
			if (!((unsigned long)addr & 0xf))
				continue;

			printf("%u. malloc(%u) returned addr = %p - ERROR\n", j, size, addr);
			error_count ++;
		}
	}

	exit(error_count);
}
