/*
 * \brief  Process monitor
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/heap.h>
#include <base/env.h>
#include <os/server.h>
#include <rom_session/connection.h>

/* local includes */
#include <top_level_type_registry.h>
#include <aggregate.h>

namespace Monitor { struct Main; }


struct Monitor::Main
{
	Entrypoint  &ep;

	Genode::Env &env;

	Genode::Rom_connection config_rom_session = { "config" };

	Top_level_type_registry const top_level_type_registry = { *env.heap() };

	Aggregate_instance top_level_aggregate_instance =
	{
		*env.parent(),
		*env.ram_session(),
		config_rom_session,
		*env.rm_session(),
		ep,
		*env.heap(),
		top_level_type_registry
	};

	Main(Entrypoint &ep, Genode::Env &env) : ep(ep), env(env) { }
};


extern "C" void wait_for_continue();

namespace Server {

	char const *name() { return "monitor_ep"; }

	size_t stack_size() { return 64*1024*sizeof(long); }

	void construct(Entrypoint &ep)
	{
		wait_for_continue();
		static Monitor::Main main(ep, *Genode::env());

		char buf[10000];
		Xml_generator xml(buf, sizeof(buf), "top_level_types", [&] () {
			main.top_level_type_registry.serialize_elements(xml); });
		printf("--- top-level types ---\n%s\n\n", buf);

		main.top_level_aggregate_instance.dump_state();

		PINF("done.");
	}
}
