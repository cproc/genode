/*
 * \brief  Test for changing configuration at runtime
 * \author Norman Feske
 * \date   2012-04-04
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <os/config.h>


void parse_config()
{
	try {
		long counter = 1;
		Genode::config()->xml_node().sub_node("counter").value(&counter);
		Genode::printf("obtained counter value %ld from config\n", counter);

	} catch (...) {
		PERR("Error while parsing the configuration");
	}
}


int main(int, char **)
{
	parse_config();

	/* register signal handler for config changes */
	Genode::Signal_receiver sig_rec;
	Genode::Signal_context sig_ctx;
	Genode::config()->sigh(sig_rec.manage(&sig_ctx));

	for (;;) {

		/* wait for config change */
		sig_rec.wait_for_signal();

		try {
			Genode::config()->reload();
			parse_config();
		} catch (Genode::Config::Invalid) {
			PERR("Error: reloading config failed");
		}
	}
	return 0;
}
