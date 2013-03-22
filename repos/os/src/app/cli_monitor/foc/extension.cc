/*
 * \brief  Fiasco.OC-specific CLI-monitor extensions
 * \author Norman Feske
 * \date   2013-03-18
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* local includes */
#include <extension.h>
#include <terminal_util.h>

/* Fiasco includes */
namespace Fiasco {
#include <l4/sys/kdebug.h>
#include <l4/sys/ipc.h>
}


static void clear_host_terminal()
{
	using namespace Fiasco;

	outstring("\e[99S");  /* scroll up */
	outstring("\e[99T");  /* scroll down */
	outstring("\e[199A"); /* move cursor up */
}


struct Kdebug_command : Command
{
	Kdebug_command() : Command("kdebug", "enter kernel debugger (via serial console)") { }

	void execute(Command_line &, Terminal::Session &terminal)
	{
		using namespace Fiasco;

		/* let kernel debugger detect the screen size */
		enter_kdebug("*#JS");

		clear_host_terminal();
		enter_kdebug("Entering kernel debugger... Press [?] for help");
		clear_host_terminal();
	}
};


struct Reboot_command : Command
{
	Reboot_command() : Command("reboot", "reboot machine") { }

	void execute(Command_line &, Terminal::Session &terminal)
	{
		using namespace Fiasco;

		clear_host_terminal();
		enter_kdebug("*#^");
	}
};


void init_extension(Command_registry &commands)
{
	static Kdebug_command kdebug_command;
	commands.insert(&kdebug_command);

	static Reboot_command reboot_command;
	commands.insert(&reboot_command);
}
