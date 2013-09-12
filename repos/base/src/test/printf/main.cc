/*
 * \brief  Testing 'printf()' with negative integer
 * \author Christian Prochaska
 * \date   2012-04-20
 *
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>

extern void print_backtrace();

void func()
{
	PDBG("calling print_backtrace()");
	print_backtrace();
	PDBG("print_backtrace() returned");
}

int main(int argc, char **argv)
{
	//Genode::printf("-1 = %d = %ld\n", -1, -1L);
PDBG("&main = %p", &main);
PDBG("&func = %p", &func);
func();
	return 0;
}

