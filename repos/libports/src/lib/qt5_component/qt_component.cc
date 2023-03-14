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
#include <libc/args.h>
#include <libc/component.h>

/* libc includes */
#include <pthread.h>
#include <pthread_np.h>
#include <stdlib.h> /* 'exit'    */

/* qt5_component includes */
#include <qt5_component/qpa_init.h>

/* initial environment for the FreeBSD libc implementation */
extern char **environ;

/* provided by the application */
extern "C" int main(int argc, char **argv, char **envp);
extern "C" void wait_for_continue();

static int argc    = 0;
static char **argv = nullptr;
static char **envp = nullptr;

static char default_argv0[] { "qt5_component" };
static char *default_argv[] { default_argv0, nullptr };

void *pthread_main(void *)
{
	exit(main(argc, argv, envp));
}

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		qpa_init(env);

		populate_args_and_env(env, argc, argv, envp);

		/* at least the executable name is required */

		if (argc == 0) {
			argc = 1;
			argv = default_argv;
		}

		environ = envp;

		bool run_main_in_pthread = true;

		if (run_main_in_pthread) {
			pthread_t main_pthread;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setname_np(&attr, "Qt main");
			pthread_create(&main_pthread, &attr, pthread_main, nullptr);
		} else {
			exit(main(argc, argv, envp));
		}
	});
}
