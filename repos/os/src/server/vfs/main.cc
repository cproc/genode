/*
 * \brief  VFS File_system server
 * \author Emery Hemingway
 * \date   2015-08-16
 */

/*
 * Copyright (C) 2015-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <vfs/file_system_factory.h>
#include <base/component.h>
#include <base/attached_rom_dataspace.h>
#include <base/sleep.h>

/* Local includes */
#include "fs_component.h"

namespace Vfs_server { struct Main; }


struct Vfs_server::Main
{
	Genode::Env &env;

	Genode::Attached_rom_dataspace config_rom { env, "config" };
	Genode::Xml_node config_node = config_rom.xml();

	Genode::Sliced_heap sliced_heap { &env.ram(), &env.rm() };
	Genode::Heap               heap { &env.ram(), &env.rm() };

	void update_config() { config_rom.update(); }

	Genode::Signal_handler<Main> _config_handler
		{ env.ep(), *this, &Main::update_config };

	Genode::Xml_node vfs_config()
	{
		try { return config_rom.xml().sub_node("vfs"); }
		catch (...) {
			Genode::error("vfs not configured");
			env.parent().exit(~0);
			Genode::sleep_forever();
		}
	}

	Vfs::Dir_file_system vfs_root
		{ env, heap, vfs_config(), Vfs::global_file_system_factory() };

	Fs_root fs_root { env, sliced_heap, vfs_root, config_rom };

	Main(Genode::Env &env) : env(env)
	{
		env.parent().announce(env.ep().manage(fs_root));
	}
};


/***************
 ** Component **
 ***************/

Genode::size_t Component::stack_size() {
	return 8*1024*sizeof(long); }

void Component::construct(Genode::Env &env) {
	static Vfs_server::Main server(env); }
