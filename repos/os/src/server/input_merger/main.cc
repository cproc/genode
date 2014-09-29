/*
 * \brief  Input event merger
 * \author Christian Prochaska
 * \date   2014-09-29
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <input/component.h>
#include <os/server.h>
#include <os/static_root.h>

namespace Input_merger
{
	struct Main;

	using Genode::Static_root;

}

/******************
 ** Main program **
 ******************/

struct Input_merger::Main
{
	Server::Entrypoint &ep;

	/*
	 * Input session provided to our client
	 */
	Input::Session_component input_session_component;

	/*
	 * Attach root interface to the entry point
	 */
	Static_root<Input::Session> input_root { ep.manage(input_session_component) };

	/**
	 * Constructor
	 */
	Main(Server::Entrypoint &ep) : ep(ep)
	{

		/*
		 * Announce service
		 */
		Genode::env()->parent()->announce(ep.manage(input_root));

	}

};

/************
 ** Server **
 ************/

namespace Server {

	char const *name() { return "input_merger_ep"; }

	size_t stack_size() { return 4*1024*sizeof(addr_t); }

	void construct(Entrypoint &ep) { static Input_merger::Main inst(ep); }
}
