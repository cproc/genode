/*
 * \brief  Simple command-line interface for managing Genode subsystems
 * \author Norman Feske
 * \date   2013-03-18
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <init/child_policy.h>
#include <os/config.h>
#include <os/child_policy_dynamic_rom.h>
#include <cap_session/connection.h>

/* local includes */
#include <line_editor.h>
#include <command_line.h>
#include <format_util.h>
#include <extension.h>
#include <ram.h>
#include <status_command.h>
#include <kill_command.h>
#include <start_command.h>
#include <help_command.h>
#include <yield_command.h>
#include <ram_command.h>

using Genode::Xml_node;


inline void *operator new (size_t size)
{
	return Genode::env()->heap()->alloc(size);
}


/******************
 ** Main program **
 ******************/

static inline Command *lookup_command(char const *buf, Command_registry &registry)
{
	Token token(buf);
	for (Command *curr = registry.first(); curr; curr = curr->next())
		if (strcmp(token.start(), curr->name(), token.len()) == 0
		 && strlen(curr->name()) == token.len())
			return curr;
	return 0;
}


static size_t ram_preservation_from_config()
{
	Genode::Number_of_bytes ram_preservation = 0;
	try {
		Genode::Xml_node node =
			Genode::config()->xml_node().sub_node("preservation");

		if (node.attribute("name").has_value("RAM"))
			node.attribute("quantum").value(&ram_preservation);
	} catch (...) { }

	return ram_preservation;
}


int main(int argc, char **argv)
{
	/* look for dynamic linker */
	try {
		static Genode::Rom_connection rom("ld.lib.so");
		Genode::Process::dynamic_linker(rom.dataspace());
	} catch (...) { }

	using Genode::Signal_context;
	using Genode::Signal_context_capability;
	using Genode::Signal_receiver;

	static Genode::Cap_connection cap;
	static Terminal::Connection   terminal;
	static Command_registry       commands;
	static Child_registry         children;
	static Process_arg_registry   process_args;

	/* initialize platform-specific commands */
	init_extension(commands);

	static Signal_receiver sig_rec;
	static Signal_context  read_avail_sig_ctx;
	terminal.read_avail_sigh(sig_rec.manage(&read_avail_sig_ctx));

	static Signal_context yield_response_sig_ctx;
	static Signal_context_capability yield_response_sig_cap =
		sig_rec.manage(&yield_response_sig_ctx);

	static Signal_context yield_broadcast_sig_ctx;
	static Signal_context resource_avail_sig_ctx;

	static Ram ram(ram_preservation_from_config(),
	               sig_rec.manage(&yield_broadcast_sig_ctx),
	               sig_rec.manage(&resource_avail_sig_ctx));

	/* initialize generic commands */
	commands.insert(new Help_command);
	Kill_command kill_command(children, process_args);
	commands.insert(&kill_command);
	commands.insert(new Start_command(ram, cap, children,
	                                  Genode::config()->xml_node(),
	                                  process_args,
	                                  yield_response_sig_cap));
	commands.insert(new Status_command(ram, children));
	commands.insert(new Yield_command(children, process_args));
	commands.insert(new Ram_command(children, process_args));

	enum { COMMAND_MAX_LEN = 1000 };
	static char buf[COMMAND_MAX_LEN];
	static Line_editor line_editor("genode> ", buf, sizeof(buf), terminal, commands);

	for (;;) {

		/* block for event, e.g., the arrival of new user input */
		Genode::Signal signal = sig_rec.wait_for_signal();

		if (signal.context() == &read_avail_sig_ctx) {

			/* supply pending terminal input to line editor */
			while (terminal.avail() && !line_editor.is_complete()) {
				char c;
				terminal.read(&c, 1);
				line_editor.submit_input(c);
			}
		}

		if (signal.context() == &yield_response_sig_ctx
		 || signal.context() == &resource_avail_sig_ctx) {

			for (Child *child = children.first(); child; child = child->next())
				child->try_response_to_resource_request();
		}

		if (signal.context() == &yield_broadcast_sig_ctx) {

			/*
			 * Compute argument of yield request to be broadcasted to all
			 * processes.
			 */
			size_t amount = 0;

			/* amount needed to reach preservation limit */
			Ram::Status ram_status = ram.status();
			if (ram_status.avail < ram_status.preserve)
				amount += ram_status.preserve - ram_status.avail;

			/* sum of pending resource requests */
			for (Child *child = children.first(); child; child = child->next())
				amount += child->requested_ram_quota();

			for (Child *child = children.first(); child; child = child->next())
				child->yield(amount, true);
		}

		if (!line_editor.is_complete())
			continue;

		Command *command = lookup_command(buf, commands);
		if (!command) {
			Token cmd_name(buf);
			tprintf(terminal, "Error: unknown command \"");
			terminal.write(cmd_name.start(), cmd_name.len());
			tprintf(terminal, "\"\n");
			line_editor.reset();
			continue;
		}

		/* validate parameters against command meta data */
		Command_line cmd_line(buf, *command);
		Token unexpected = cmd_line.unexpected_parameter();
		if (unexpected) {
			tprintf(terminal, "Error: unexpected parameter \"");
			terminal.write(unexpected.start(), unexpected.len());
			tprintf(terminal, "\"\n");
			line_editor.reset();
			continue;
		}
		command->execute(cmd_line, terminal);

		/*
		 * The command might result in a change of the RAM usage. Validate
		 * that the preservation is satisfied.
		 */
		ram.validate_preservation();
		line_editor.reset();
	}

	return 0;
}
