/*
 * \brief  USB raw session file system
 * \author Christian Prochaska
 * \date   2016-11-02
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <vfs/file_system_factory.h>

/* local includes */
#include <usb_file_system.h>

namespace Vfs { struct Usb_file_system_factory; }


struct Vfs::Usb_file_system_factory : Vfs::File_system_factory
{
	Vfs::File_system *create(Genode::Env&, Genode::Allocator &alloc,
	                         Genode::Xml_node  node) override
	{
		Genode::log("creating USB file system");
		return new (alloc) Vfs::Usb_file_system(node);
	}
};


extern "C" Vfs::File_system_factory *vfs_file_system_factory(void)
{
	static Vfs::Usb_file_system_factory factory;
	return &factory;
}
