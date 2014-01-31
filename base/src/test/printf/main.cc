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
#include <base/thread.h>

using namespace Genode;

struct Test_thread : Thread<8192>
{

	void entry()
	{
		Genode::printf("%f\n", 1.0);
	}

	Test_thread() : Thread<8192>("test") { }

};

int main(int argc, char **argv)
{
	static Test_thread t;
	t.start();
	t.join();
	return 0;
}

