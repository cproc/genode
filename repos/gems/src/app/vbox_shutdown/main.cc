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
#include <base/sleep.h>
#include <os/reporter.h>
#include <timer_session/connection.h>

using namespace Genode;

struct Main
{
	Env &_env;

	Attached_rom_dataspace _runtime_state_rom { _env, "runtime/state" };
	Reporter               _system_config_reporter { _env, "system" };	
	Reporter               _backdrop_init_config_reporter { _env, "config", "backdrop_init_config" };
	Timer::Connection      _timer { _env };

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

				{
					/* start backdrop and let it show the shutdown image */

					Reporter::Xml_generator xml(_backdrop_init_config_reporter, [&] () {

						xml.node("parent-provides", [&] () {
							xml.node("service", [&] () {
								xml.attribute("name", "ROM");
							});
							xml.node("service", [&] () {
								xml.attribute("name", "PD");
							});
							xml.node("service", [&] () {
								xml.attribute("name", "RM");
							});
							xml.node("service", [&] () {
								xml.attribute("name", "CPU");
							});
							xml.node("service", [&] () {
								xml.attribute("name", "LOG");
							});
							xml.node("service", [&] () {
								xml.attribute("name", "Nitpicker");
							});
						});

						xml.node("default-route", [&] () {
							xml.node("any-service", [&] () {
								xml.node("parent", [] () {});
							});
						});

						xml.node("start", [&] () {
							xml.attribute("name", "backdrop");
							xml.attribute("caps", "100");
							xml.node("resource", [&] () {
								xml.attribute("name", "RAM");
								xml.attribute("quantum", "12M");
							});
							xml.node("config", [&] () {
								xml.node("vfs", [&] () {
									xml.node("rom", [&] () {
										xml.attribute("name", "shutdown.png");
									});
								});
								xml.node("image", [&] () {
									xml.attribute("png", "shutdown.png");
								});
							});
						});
					});
				}

				/* wait a moment to get the shutdown image shown on screen */

				_timer.msleep(1000);

				/* trigger the shutdown */

				{
					Reporter::Xml_generator xml(_system_config_reporter, [&] () {
						xml.attribute("state", "poweroff");
					});
				}

				Genode::sleep_forever();
			}
		});
	}

	Main(Env &env) : _env(env)
	{
		_system_config_reporter.enabled(true);
		_backdrop_init_config_reporter.enabled(true);
		_runtime_state_rom.sigh(_runtime_state_handler);
		_handle_runtime_state();
	}
};


void Component::construct(Genode::Env &env)
{
	static Main main(env);
}
