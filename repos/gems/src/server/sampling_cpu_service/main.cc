/*
 * \brief  Sampling CPU service
 * \author Christian Prochaska
 * \date   2016-01-15
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <cpu_session/cpu_session.h>
#include <os/attached_dataspace.h>
#include <os/config.h>
#include <os/server.h>
#include <os/static_root.h>
#include <timer_session/connection.h>
#include <util/list.h>

/* local includes */
#include "cpu_root.h"
#include "cpu_session_component.h"

Cpu_session_component *cpu_session_component;
Genode::Thread_capability thread_cap;

namespace Sampling_cpu_service
{
	struct Main;
}


/******************
 ** Main program **
 ******************/

struct Sampling_cpu_service::Main
{
	Server::Entrypoint &ep;

	Cpu_root cpu_root;

	Timer::Connection timer;

	void handle_timeout(unsigned)
	{
		if (cpu_session_component)
			cpu_session_component->sample();
	}

	Signal_rpc_member<Main> timeout_dispatcher =
		{ ep, *this, &Main::handle_timeout };

	void handle_config_update(unsigned)
	{
		Genode::config()->reload();
	}

	Signal_rpc_member<Main> config_update_dispatcher =
		{ ep, *this, &Main::handle_config_update};

	/**
	 * Constructor
	 */
	Main(Server::Entrypoint &ep)
	: ep(ep),
	  cpu_root(ep, Genode::env()->heap())
	{
		/*
		 * Apply initial configuration
		 */
		handle_config_update(0);

		/*
		 * Register signal handlers
		 */
		Genode::config()->sigh(config_update_dispatcher);
		timer.sigh(timeout_dispatcher);

		timer.trigger_periodic(1*1000*1000); /* 1s */

		/*
		 * Announce service
		 */
		Genode::env()->parent()->announce(ep.manage(cpu_root));
	}

};


/************
 ** Server **
 ************/

namespace Server {

	char const *name() { return "sampling_cpu_service_ep"; }

	size_t stack_size() { return 4*1024*sizeof(addr_t); }

	void construct(Entrypoint &ep) { static Sampling_cpu_service::Main inst(ep); }
}
