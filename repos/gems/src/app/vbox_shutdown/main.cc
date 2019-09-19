/*
 * \brief  Shut down PC on VirtualBox exit
 * \author Christian Prochaska
 * \date   2019-09-11
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <os/reporter.h>

using namespace Genode;

struct Main
{
	Env &_env;

	Attached_rom_dataspace _runtime_state_rom { _env, "runtime/state" };
	Reporter               _system_config_reporter { _env, "system" };

	Signal_handler<Main> _runtime_state_handler {
		_env.ep(), *this, &Main::_handle_runtime_state };

	void _handle_runtime_state()
	{
		_runtime_state_rom.update();

		Xml_node state = _runtime_state_rom.xml();
		
		state.for_each_sub_node("child", [&] (Xml_node arg_node) {
			typedef String<64> Name;
			if ((arg_node.attribute_value("name", Name()) == "vbox5-nova-sculpt") &&
				arg_node.has_attribute("exited")) {

				Genode::log("VirtualBox exited, turning power off...");

				Reporter::Xml_generator xml(_system_config_reporter, [&] () {
					xml.attribute("state", "poweroff");
				});
			}
		});
	}

	Main(Env &env) : _env(env)
	{
		_system_config_reporter.enabled(true);
		_runtime_state_rom.sigh(_runtime_state_handler);
		_handle_runtime_state();
	}
};


void Component::construct(Genode::Env &env)
{
	static Main main(env);
}
