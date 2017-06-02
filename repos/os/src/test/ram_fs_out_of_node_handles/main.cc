/*
 * \brief  ram_fs node allocation test
 * \author Christian Prochaska
 * \date   2017-06-02
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/component.h>
#include <base/heap.h>
#include <file_system_session/connection.h>

using namespace Genode;


struct Main
{
		Heap                    heap;
		Allocator_avl           fs_alloc;
		File_system::Connection fs;

		Main(Genode::Env &env)
		: heap(env.ram(), env.rm()),
		  fs_alloc(&heap),
		  fs(env, fs_alloc, "", "/", true, 128*1024)
		{
			File_system::Dir_handle root_dir = fs.dir("/", false);

			fs.file(root_dir, "test", File_system::WRITE_ONLY, true);

			for (;;) {
				fs.file(root_dir, "test", File_system::WRITE_ONLY, false);
			}
		}

};

void Component::construct(Genode::Env &env)
{
	static Main main(env);
}
