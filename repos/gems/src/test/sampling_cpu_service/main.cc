/*
 * \brief  Test for the sampling CPU service
 * \author Christian Prochaska
 * \date   2016-01-18
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/thread.h>

void __attribute((noinline)) func1()
{
	volatile unsigned long i;
	for (i = 0; i < 10000000; i++) ;
}

void __attribute((noinline)) func2()
{
	volatile unsigned long i;
	for (i = 0; i < 10000000; i++) ;
}

int main(int argc, char *argv[])
{
	PDBG("Test started");

	for (;;) {
		func1();
		func2();
	}

	return 0;
}
