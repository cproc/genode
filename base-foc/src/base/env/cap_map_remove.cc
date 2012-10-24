/*
 * \brief  Mapping of Genode's capability names to kernel capabilities.
 * \author Stefan Kalkowski
 * \date   2010-12-06
 *
 * This is a Fiasco.OC-specific addition to the process enviroment.
 */

/*
 * Copyright (C) 2010-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/cap_map.h>
#include <base/snprintf.h>

namespace Fiasco {
#include <l4/sys/kdebug.h>
#include <l4/sys/debugger.h>
}
extern unsigned int gettid();
void Genode::Capability_map::remove(Genode::Cap_index* i)
{
	using namespace Genode;
	using namespace Fiasco;

	Lock_guard<Spin_lock> guard(_lock);
	char buf[128];
	snprintf(buf, sizeof(buf), "%lx %u: remove: i = %p\n", l4_debugger_global_id(L4_BASE_TASK_CAP),  gettid(), i);
	Fiasco::outstring(buf);

	if (i) {
		Cap_index* e = _tree.first() ? _tree.first()->find_by_id(i->id()) : 0;
		if (e == i)
			_tree.remove(i);
		cap_idx_alloc()->free(i, 1);
	}
}
