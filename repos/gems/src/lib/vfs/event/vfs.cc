/*
 * \brief  Event file system
 * \author Christian Prochaska
 * \date   2020-11-26
 */

/*
 * Copyright (C) 2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <vfs/file_system_factory.h>

/* local includes */
#include <vfs_event.h>


struct Event_factory : Vfs::File_system_factory
{
	Vfs::File_system *create(Vfs::Env &env, Genode::Xml_node node) override
	{
		return new (env.alloc()) Event_file_system(env.env(), env.alloc(), node);
	}
};


extern "C" Vfs::File_system_factory *vfs_file_system_factory(void)
{
	static Event_factory factory;
	return &factory;
}
