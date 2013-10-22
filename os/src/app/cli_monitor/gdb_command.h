/*
 * \brief  Gdb command
 * \author Norman Feske
 * \author Christian Prochaska
 * \date   2013-03-18
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _GDB_COMMAND_H_
#define _GDB_COMMAND_H_

/* Genode includes */
#include <util/xml_node.h>

/* local includes */
#include <child_registry.h>
#include <format_util.h>
#include <process_arg_registry.h>
#include <ram.h>

struct Gdb_command : Command
{
	typedef Genode::Xml_node                  Xml_node;
	typedef Genode::Signal_context_capability Signal_context_capability;

	Ram                      &_ram;
	Child_registry           &_children;
	Genode::Cap_session      &_cap;
	Xml_node                  _config;
	Process_arg_registry     &_process_args;
	List<Argument>            _arguments;
	Signal_context_capability _yield_response_sigh_cap;

	Gdb_command(Ram &ram, Genode::Cap_session &cap, Child_registry &children,
	            Xml_node config, Process_arg_registry &process_args,
	            Signal_context_capability yield_response_sigh_cap)
	:
		Command("gdb", "create new subsystem with GDB"),
		_ram(ram), _children(children), _cap(cap), _config(config),
		_process_args(process_args),
		_yield_response_sigh_cap(yield_response_sigh_cap)
	{
		/* scan config for possible subsystem arguments */
		try {
			Xml_node node = _config.sub_node("subsystem");
			for (;; node = node.next("subsystem")) {

				char name[Parameter::NAME_MAX_LEN];
				try { node.attribute("name").value(name, sizeof(name)); }
				catch (Xml_node::Nonexistent_attribute) {
					PWRN("Missing name in '<subsystem>' configuration");
					continue;
				}

				char   const *prefix     = "config: ";
				size_t const  prefix_len = strlen(prefix);

				char help[Parameter::SHORT_HELP_MAX_LEN + prefix_len];
				strncpy(help, prefix, ~0);
				try { node.attribute("help").value(help + prefix_len,
				                                   sizeof(help) - prefix_len); }
				catch (Xml_node::Nonexistent_attribute) {
					PWRN("Missing help in '<subsystem>' configuration");
					continue;
				}

				_arguments.insert(new Argument(name, help));
			}
		} catch (Xml_node::Nonexistent_sub_node) { /* end of list */ }

#if 0
		add_parameter(new Parameter("--count",     Parameter::NUMBER, "number of instances"));
		add_parameter(new Parameter("--ram",       Parameter::NUMBER, "initial RAM quota"));
		add_parameter(new Parameter("--ram-limit", Parameter::NUMBER, "limit for expanding RAM quota"));
		add_parameter(new Parameter("--verbose",   Parameter::VOID,   "show diagnostics"));
#endif
	}

	/**
	 *  
	 */
	Xml_node _gdb_config_node(const char *binary_name,
	                          const char *target_config_addr,
	                          const size_t target_config_size)
	{
		enum { BUF_SIZE = 3*1024 };
		static char buf[BUF_SIZE];

		char target_config[target_config_size + 1];
		if (target_config_addr)
			Genode::snprintf(target_config, sizeof(target_config), target_config_addr);
		else
			target_config[0] = '\0';

		int len = Genode::snprintf(buf, sizeof(buf), "\
		<config> \
			<parent-provides> \
				<service name=\"ROM\"/> \
				<service name=\"RAM\"/> \
				<service name=\"CAP\"/> \
				<service name=\"PD\"/> \
				<service name=\"RM\"/> \
				<service name=\"CPU\"/> \
				<service name=\"LOG\"/> \
				<service name=\"SIGNAL\"/> \
				<service name=\"Timer\"/> \
				<service name=\"Terminal\"/> \
			</parent-provides> \
			<start name=\"terminal_gdb\"> \
				<binary name=\"terminal_crosslink\"/> \
				<resource name=\"RAM\" quantum=\"1M\"/> \
				<provides> <service name=\"Terminal\"/> </provides> \
				<route> \
					<any-service><parent/><any-child/></any-service> \
				</route> \
			</start> \
			<start name=\"gdb_monitor\"> \
				<resource name=\"RAM\" quantum=\"10M\"/> \
				<route> \
					<service name=\"Terminal\"><child name=\"terminal_gdb\"/></service> \
					<any-service> <any-child/> <parent/> </any-service> \
				</route> \
				<config> \
					<target name=\"%s\"> \
						%s \
					</target> \
					<preserve name=\"RAM\" quantum=\"5M\"/> \
				</config> \
			</start> \
			<start name=\"noux\"> \
				<resource name=\"RAM\" quantum=\"1G\"/> \
				<route> \
					<service name=\"Terminal\"> \
						<if-arg key=\"label\" value=\"\"/><parent/> \
					</service> \
					<service name=\"Terminal\"> \
						<if-arg key=\"label\" value=\"noux(terminal_fs)\"/><child name=\"terminal_gdb\"/> \
					</service> \
					<any-service> <any-child/> <parent/> </any-service> \
				</route> \
				<config> \
					<fstab> \
						<tar name=\"gdb.tar\" at=\"/\"/> \
						<dir name=\"dev\"><terminal name=\"gdb\"/></dir> \
						<dir name=\"gdb\"> \
							<tar name=\"%s.tar\"/> \
							<dir name=\"src\"> <tar name=\"%s-src.tar\"/> </dir> \
						</dir> \
					</fstab> \
					<start name=\"/bin/genode-arm-gdb\"> \
						<arg value=\"/gdb/%s\"/> \
						<arg value=\"-ex\" /><arg value=\"set interactive-mode off\" /> \
						<arg value=\"-ex\" /><arg value=\"directory /gdb/src\" /> \
						<arg value=\"-ex\" /><arg value=\"target remote /dev/gdb\" /> \
						<arg value=\"-ex\" /><arg value=\"symbol-file /gdb/ld.lib.so\" /> \
						<arg value=\"-ex\" /><arg value=\"b call_main\" /> \
						<arg value=\"-ex\" /><arg value=\"c\" /> \
						<arg value=\"-ex\" /><arg value=\"delete 1\" /> \
						<arg value=\"-ex\" /><arg value=\"symbol-file /gdb/%s\" /> \
						<arg value=\"-ex\" /><arg value=\"b main\" /> \
						<arg value=\"-ex\" /><arg value=\"set solib-search-path /gdb\" /> \
						<arg value=\"-ex\" /><arg value=\"sharedlibrary\" /> \
						<arg value=\"-ex\" /><arg value=\"c\" /> \
						<arg value=\"-ex\" /><arg value=\"delete 2\" /> \
						<arg value=\"-ex\" /><arg value=\"set interactive-mode auto\" /> \
					</start> \
				</config> \
			</start> \
		</config> \
		",
		binary_name,
		target_config,
		binary_name,
		binary_name,
		binary_name,
		binary_name);

		PDBG("len = %d", len);

		return Xml_node(buf, len + 1);
	}

	/**
	 * Lookup subsystem in config
	 */
	Xml_node _subsystem_node(char const *name)
	{
		Xml_node node = _config.sub_node("subsystem");
		for (;; node = node.next("subsystem")) {
			if (node.attribute("name").has_value(name))
				return node;
		}
	}

	void execute(Command_line &cmd, Terminal::Session &terminal)
	{
		size_t count = 1;
		Genode::Number_of_bytes ram = 0;
		Genode::Number_of_bytes ram_limit = 0;

		char name[128];
		name[0] = 0;
		if (cmd.argument(0, name, sizeof(name)) == false) {
			tprintf(terminal, "Error: no configuration name specified\n");
			return;
		}

		char buf[128];
		if (cmd.argument(1, buf, sizeof(buf))) {
			tprintf(terminal, "Error: unexpected argument \"%s\"\n", buf);
			return;
		}

		/* check if a configuration for the subsystem exists */
		try { _subsystem_node(name); }
		catch (Xml_node::Nonexistent_sub_node) {
			tprintf(terminal, "Error: no configuration for \"%s\"\n", name);
			return;
		}

		/* read default RAM quota from config */
		try {
			Xml_node rsc = _subsystem_node(name).sub_node("resource");
			for (;; rsc = rsc.next("resource")) {
				if (rsc.attribute("name").has_value("RAM")) {
					rsc.attribute("quantum").value(&ram);

					if (rsc.has_attribute("limit"))
						rsc.attribute("limit").value(&ram_limit);
					break;
				}
			}
		} catch (...) { }

#if 0
		cmd.parameter("--count",     count);*/
		cmd.parameter("--ram",       ram);
		cmd.parameter("--ram-limit", ram_limit);
#endif

		/* add quota for GDB-related processes */
		ram = ram + 15*1024*1024;

		/* acount for cli_monitor local metadata */
		size_t preserve_ram = 100*1024;
		if (count * (ram + preserve_ram) > Genode::env()->ram_session()->avail()) {
			tprintf(terminal, "Error: RAM quota exceeds available quota\n");
			return;
		}

		bool const verbose = cmd.parameter_exists("--verbose");

		/*
		 * Determine binary name
		 *
		 * Use subsystem name by default, override with '<binary>' declaration.
		 */
		char binary_name[128];
		strncpy(binary_name, name, sizeof(binary_name));
		try {
			Xml_node bin = _subsystem_node(name).sub_node("binary");
			bin.attribute("name").value(binary_name, sizeof(binary_name));
		} catch (...) { }

		for (unsigned i = 0; i < count; i++) {

			/* generate unique child name */
			char label[Child_registry::CHILD_NAME_MAX_LEN];
			_children.unique_child_name(name, label, sizeof(label));

			tprintf(terminal, "starting new subsystem '%s'\n", label);

			if (verbose) {
				tprintf(terminal, "  RAM quota: ");
				tprint_bytes(terminal, ram);
				tprintf(terminal,"\n");
				if (ram_limit) {
					tprintf(terminal, "  RAM limit: ");
					tprint_bytes(terminal, ram_limit);
					tprintf(terminal,"\n");
				}
				tprintf(terminal, "     binary: %s\n", binary_name);
			}

			Child *child = 0;
			try {
				child = new (Genode::env()->heap())
					Child(_ram, label, "init", _cap, ram, ram_limit,
					      _yield_response_sigh_cap);
			}
			catch (Genode::Rom_connection::Rom_connection_failed) {
				tprintf(terminal, "Error: could not obtain ROM module \"%s\"\n",
				        binary_name);
				return;
			}
			catch (Child::Quota_exceeded) {
				tprintf(terminal, "Error: insufficient memory, need ");
				tprint_bytes(terminal, ram + Child::DONATED_RAM_QUOTA);
				tprintf(terminal, ", have ");
				tprint_bytes(terminal, Genode::env()->ram_session()->avail());
				tprintf(terminal, "\n");
				return;
			}
			catch (Genode::Allocator::Out_of_memory) {
				tprintf(terminal, "Error: could not allocate meta data, out of memory\n");
				return;
			}

			/* configure child */
			try {
				const char *target_config_addr = 0;
				size_t target_config_size = 0;
				try { 
					Xml_node target_config_node = _subsystem_node(name).sub_node("config");
					target_config_addr = target_config_node.addr();
					target_config_size = target_config_node.size();
				} catch (...) { }
				Xml_node config_node = _gdb_config_node(binary_name,
				                                        target_config_addr,
				                                        target_config_size);
				child->configure(config_node.addr(), config_node.size());
				if (verbose)
					tprintf(terminal, "     config: inline\n");
			} catch (...) {
				if (verbose)
					tprintf(terminal, "     config: none\n");
			}

			_process_args.list.insert(&child->argument);
			_children.insert(child);
			child->start();
		}
	}

	List<Argument> &arguments() { return _arguments; }
};

#endif /* _GDB_COMMAND_H_ */
