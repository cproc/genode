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
#include <stdlib.h> /* 'exit'   */

/* Qt includes */
#include <qpluginwidget/qpluginwidget.h>

/* provided by the application */
extern "C" int main(int argc, char const **argv);

/* provided by the QPA plugin */
extern void initialize_qpa_plugin(Genode::Env &);

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		initialize_qpa_plugin(env);
		QPluginWidget::set_env(env);

		int argc = 1;
		char const *argv[] = { "arora", 0 };

		exit(main(argc, argv));
	});
}
