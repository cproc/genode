/*
 * \brief  JDB backtrace test
 * \author Christian Prochaska
 * \date   2013-02-22
 *
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <l4/sys/kdebug.h>

void func3()
{
	enter_kdebug("func3()");
}


void func2()
{
	func3();
}


void func1()
{
	func2();
}


int main(int argc, char **argv)
{
	func1();

	return 0;
}
