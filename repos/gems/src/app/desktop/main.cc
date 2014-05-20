/*
 * \brief  Desktop environment
 * \author Norman Feske
 * \date   2014-01-06
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <os/server.h>
#include <nitpicker_session/client.h>
#include <nitpicker_view/client.h>
#include <nitpicker_view/capability.h>
#include <framebuffer_session/client.h>
#include <cap_session/connection.h>

/* local includes */
#include <single_session_service.h>
#include <nitpicker_slave.h>
#include <report_rom_slave.h>
#include <decorator_nitpicker.h>
#include <local_reporter.h>
#include <decorator_slave.h>
#include <window_layouter_slave.h>
#include <nitpicker.h>

namespace Desktop {

	class Main;

	using Genode::size_t;
	using Genode::env;
}


struct Desktop::Main
{
	Server::Entrypoint ep;

	Genode::Cap_connection cap;

	Report_rom_slave report_rom_slave = { cap, *env()->ram_session() };

	Rom_session_capability window_list_rom   = report_rom_slave.rom_session("window_list");
	Rom_session_capability window_layout_rom = report_rom_slave.rom_session("window_layout");
	Rom_session_capability pointer_rom       = report_rom_slave.rom_session("pointer");
	Rom_session_capability hover_rom         = report_rom_slave.rom_session("hover");

	/* pointer position reported by nitpicker */
	Capability<Report::Session> pointer_report = report_rom_slave.report_session("pointer");

	/* hovered element reported by decorator */
	Capability<Report::Session> hover_report = report_rom_slave.report_session("hover");

	Capability<Report::Session> window_list_report = report_rom_slave.report_session("window_list");
	Local_reporter window_list_reporter = { "window_list", window_list_report };

	Capability<Report::Session> window_layout_report = report_rom_slave.report_session("window_layout");

	Input::Session_component window_layouter_input;

	Nitpicker_slave nitpicker_slave = { cap, *env()->ram_session(), pointer_report };

	Window_registry window_registry { *env()->heap(), window_list_reporter };

	Nitpicker::Root nitpicker_root { ep, nitpicker_slave, window_registry,
	                                 *env()->heap(), *env()->ram_session() };

	Decorator_nitpicker_service decorator_nitpicker_service {
		nitpicker_slave, ep, *env()->heap(), *env()->ram_session(),
		window_layouter_input, nitpicker_root };

	Window_layouter_slave window_layouter_slave = {
		cap, *env()->ram_session(), window_list_rom, hover_rom,
		ep.manage(window_layouter_input), window_layout_report };

	Decorator_slave decorator_slave = {
		cap, decorator_nitpicker_service, *env()->ram_session(),
		window_layout_rom, pointer_rom, hover_report };

	Main(Server::Entrypoint &ep) : ep(ep)
	{
		/* initially report an empty window list */
		Local_reporter::Xml_generator xml(window_list_reporter, [&] () { });
	}
};


/************
 ** Server **
 ************/

namespace Server {

	char const *name() { return "desktop_ep"; }

	size_t stack_size() { return 4*1024*sizeof(long); }

	void construct(Entrypoint &ep)
	{
		static Desktop::Main desktop(ep);
	}
}
