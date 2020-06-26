/*
 * \brief  Entry point for Qt applications with a main() function
 * \author Christian Prochaska
 * \date   2017-05-22
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <libc/component.h>

/* libc includes */
#include <dlfcn.h>  /* 'dlopen'  */
#include <stdio.h>  /* 'fprintf' */
#include <stdlib.h> /* 'exit'    */
#include <unistd.h> /* 'access'  */

/* provided by the application */
extern "C" int main(int argc, char const **argv);

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		/*
		 * initialize the QPA plugin
		 *
		 * when Qt loads the plugin again, it will get the same handle
		 */

		char const *qpa_plugin = "/qt/plugins/platforms/libqgenode.lib.so";

		void *qpa_plugin_handle = nullptr;

		/* check existence with 'access()' first to avoid ld error messages */
		if (access(qpa_plugin, F_OK) == 0)
			qpa_plugin_handle = dlopen(qpa_plugin, RTLD_LAZY);
		
		if (qpa_plugin_handle) {

			typedef void (*initialize_qpa_plugin_t)(Genode::Env &);

			initialize_qpa_plugin_t initialize_qpa_plugin = 
				(initialize_qpa_plugin_t) dlsym(qpa_plugin_handle,
				                                "initialize_qpa_plugin");

			if (!initialize_qpa_plugin) {
				fprintf(stderr, "Could not find 'initialize_qpa_plugin' \
				                 function in QPA plugin\n");
				dlclose(qpa_plugin_handle);
				exit(1);
			}

			initialize_qpa_plugin(env);
		}

		int argc = 1;
		char const *argv[] = { "qt5_app", 0 };

		int exit_value = main(argc, argv);

		if (qpa_plugin_handle)
			dlclose(qpa_plugin_handle);

		exit(exit_value);
	});
}
