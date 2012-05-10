/*
 * \brief  Test if the valgrind_wrapper works
 * \author Christian Prochaska
 * \date   2012-05-10
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>

using namespace Genode;

int main(int, char **)
{
	printf("--- valgrind-wrapper test ---\n");

	/* 'volatile' to prevent the compiler from optimizing everything out */
	volatile int x;

	/* provoke a 'Conditional jump or move depends on uninitialised value(s)' message */
	if (x == 0)
		x = 1;

	printf("--- finished valgrind-wrapper test ---\n");
	return 0;
}
