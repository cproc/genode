/*
 * \brief  Component which initiates the download of 'dbg' archives
 *         based on the current runtime/monitor configuration
 * \author Christian Prochaska
 * \date   2023-12-04
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/heap.h>
#include <base/log.h>
#include <os/reporter.h>
#include <os/vfs.h>

using namespace Genode;

struct Main
{
	static constexpr char const *runtime_config_path = "/config/managed/runtime";

	Genode::Env &_env;
	Heap         _heap { _env.ram(), _env.rm() };

	Attached_rom_dataspace _config { _env, "config" };
	Attached_rom_dataspace _build_info { _env, "build_info" };
	Session_label          _base_archive { };

	Root_directory _root_dir { _env, _heap, _config.xml().sub_node("vfs") };
	Readonly_file  _runtime_config { _root_dir, runtime_config_path };

	Watch_handler<Main> runtime_config_watch_handler {
		_root_dir, runtime_config_path,
		*this, &Main::_handle_runtime_config_update };

	Expanding_reporter _installation { _env, "installation", "installation" };

	void _handle_runtime_config_update()
	{
Genode::log("_handle_runtime_config_update()");

		size_t buf_size = _root_dir.file_size(runtime_config_path);
		char *buf = (char *)_heap.alloc(buf_size);

		Byte_range_ptr range(buf, buf_size);

		with_xml_file_content(_runtime_config, range, [&] (Xml_node const config) {

//Genode::log(config);

			config.with_sub_node("monitor", [&] (Xml_node const &monitor) {

				_installation.generate([&] (Xml_generator &xml) {

//Genode::log(monitor);

				monitor.for_each_sub_node("policy", [&] (Xml_node const &policy) {

//Genode::log(policy);

					Session_label label {
						policy.attribute_value("label", Session_label::String()) };

Genode::log("monitor policy label: ", label);
#if 0
					if (_debug_dir.directory_exists(label))
						return;

Genode::log("new component: ", label);

					/* create symlinks */

					_debug_dir.create_sub_directory(label);

					Directory component_dir { _debug_dir, label };
#endif
					config.for_each_sub_node("start", [&] (Xml_node const &start) {

						if (start.attribute_value("name", Session_label::String()) !=
						    label)
							return;

// Genode::log(start);

					    start.with_sub_node("route", [&] (Xml_node const &route) {

							route.for_each_sub_node("service", [&] (Xml_node const &service) {

// Genode::log("service: ", service);

								if (service.attribute_value("name", String<8>()) != "ROM")
									return;

								Session_label rom_session_label;

								if (service.attribute_value("label_last", Session_label::String()) == "ld.lib.so") {

									rom_session_label = { _base_archive, "/ld.lib.so" };

								} else {

									service.with_sub_node("child", [&] (Xml_node const &child) {

										if (child.attribute_value(
										    "name", String<16>()) != "depot_rom")
											return;

										rom_session_label = child.attribute_value("label", Session_label::String());
									}, [&] () {
										Genode::warning("child XML node not found");
										return;
									});
								}

								Vfs::Absolute_path rom_session_label_path { rom_session_label };

Genode::log("rom_session_label_path: ", rom_session_label_path);

								Vfs::Absolute_path depot_user_path { rom_session_label_path };
								while (!depot_user_path.has_single_element())
									depot_user_path.strip_last_element();

Genode::log("depot_user_path: ", depot_user_path);

								Vfs::Absolute_path depot_type_path { rom_session_label_path };
								depot_type_path.strip_prefix(depot_user_path.string());
								while (!depot_type_path.has_single_element())
									depot_type_path.strip_last_element();

Genode::log("depot_type_path: ", depot_type_path);

								if (depot_type_path != "/bin")
									return;

								Vfs::Absolute_path depot_component_path { rom_session_label_path };
								depot_component_path.strip_prefix(
									Directory::join(depot_user_path, depot_type_path).string());

Genode::log("depot_component_path: ", depot_component_path);

								Vfs::Absolute_path bin_installation_path { depot_user_path };
								bin_installation_path.append("/bin");
								bin_installation_path.append(depot_component_path.string());
								bin_installation_path.strip_last_element();
Genode::log("bin_installation_path: ", bin_installation_path);

								xml.node("archive", [&] () {
									xml.attribute("path", &bin_installation_path.string()[1]);
									xml.attribute("source", "no");
								});

								Vfs::Absolute_path dbg_installation_path { depot_user_path };
								dbg_installation_path.append("/dbg");
								dbg_installation_path.append(depot_component_path.string());
								dbg_installation_path.strip_last_element();
Genode::log("dbg_installation_path: ", dbg_installation_path);

								xml.node("archive", [&] () {
									xml.attribute("path", &dbg_installation_path.string()[1]);
									xml.attribute("source", "no");
								});
#if 0
								Vfs::Absolute_path depot_bin_path { "/depot" };
								depot_bin_path.append(depot_user_path.string());
								depot_bin_path.append("/bin");
								depot_bin_path.append(depot_component_path.string());
								Vfs::Absolute_path bin_file_path { depot_component_path };
								bin_file_path.keep_only_last_element();

Genode::log("depot_bin_path: ", depot_bin_path);
Genode::log("bin_file_path: ", bin_file_path);

								component_dir.create_symlink(bin_file_path,
									depot_bin_path);

Genode::log("read_symlink(): ", component_dir.read_symlink(bin_file_path));

								Vfs::Absolute_path depot_dbg_path { "/depot" };
								depot_dbg_path.append(depot_user_path.string());
								depot_dbg_path.append("/dbg");
								depot_dbg_path.append(depot_component_path.string());
								depot_dbg_path.append(".debug");
								Vfs::Absolute_path debug_file_path { depot_dbg_path };
								debug_file_path.keep_only_last_element();

Genode::log("depot_dbg_path: ", depot_dbg_path);
Genode::log("debug_file_path: ", debug_file_path);

								component_dir.create_symlink(debug_file_path,
									depot_dbg_path);

Genode::log("read_symlink(): ", component_dir.read_symlink(debug_file_path));
#endif
							});
						}, [&] () {
							Genode::error("route XML node not found");
						});
					});
				});
				});

//				_remove_outdated_debug_directories(monitor);

			}, [&] () {
				Genode::error("monitor XML node not found");
			});
		});

		destroy(_heap, buf);
Genode::log("_handle_runtime_config_update() finished");
	}

	Main(Genode::Env &env)
	: _env(env)
	{
		Genode::log("dbg_download");
		Genode::log("build_info: ", _build_info.xml());
		_base_archive = _build_info.xml().attribute_value("base", Session_label::String());
		Genode::log("_base_archive: ", _base_archive);
		_handle_runtime_config_update();
	}
};


void Component::construct(Genode::Env &env)
{
	static Main main(env);
}
