/*
 * \brief  Entry point for Qt applications with a main() function
 * \author Christian Prochaska
 * \date   2017-05-22
 */

/*
 * Copyright (C) 2017-2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <libc/component.h>

/* libc includes */
#include <stdlib.h> /* 'exit'    */

/* qt5_component includes */
#include <qt5_component/qpa_init.h>

/* provided by the application */
extern "C" int main(int argc, char const **argv);

extern "C" void wait_for_continue();
void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		qpa_init(env);

#if 1
		int argc = 7;
		char const *argv[] = { "qt5_app",
		                       "--single-process",
		                       "--disable-gpu",
		                       "--enable-logging",
		                       "--log-level=3",
		                       "--v=0",
		                       "--no-sandbox",
		                       0 };
#else
		int argc = 1;
		char const *argv[] = { "qt5_app",
		                       0 };
#endif
wait_for_continue();
		int exit_value = main(argc, argv);

//Genode::error("main() returned");

		exit(exit_value);
	});
}
