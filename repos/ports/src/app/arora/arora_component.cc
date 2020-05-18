/*
 * \brief  Entry point for Arora
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

/* Qt includes */
#include <qpluginwidget/qpluginwidget.h>

/* provided by the application */
extern "C" int main(int argc, char const **argv);

extern void initialize_qt_gui(Genode::Env &);

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		/* initialize the QPA plugin */

		void *qpa_plugin_handle =
			dlopen("/qt/plugins/platforms/qt5_qpa_nitpicker.lib.so",
			       RTLD_LAZY);
		
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

		/* initialize the plugin widget */

		QPluginWidget::env(env);


		int argc = 3;
		char const *argv[] = { "arora",
		                       "-platformpluginpath",
		                       "/qt/plugins/platforms",
		                       0 };

		int exit_value = main(argc, argv);

		if (qpa_plugin_handle)
			dlclose(qpa_plugin_handle);

		exit(exit_value);
	});
}
