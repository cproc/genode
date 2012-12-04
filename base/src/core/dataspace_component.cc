/*
 * \brief   Dataspace component
 * \date    2006-09-18
 * \author  Christian Helmuth
 */

/*
 * Copyright (C) 2006-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <dataspace_component.h>
#include <rm_session_component.h>

using namespace Genode;


void Dataspace_component::attached_to(Rm_region *region)
{
	Lock::Guard lock_guard(_lock);
	_regions.insert(region);
}


bool Dataspace_component::detached_from(Rm_region *region)
{
	Lock::Guard lock_guard(_lock);
	return _regions.remove(region);
}


Dataspace_component::~Dataspace_component()
{
	_lock.lock();

	/* remove from all regions */
	while (Rm_region *r = _regions.first()) {
		_regions.remove(r);

		_lock.unlock();
		r->session()->_detach((void *)r->base(), true);
		_lock.lock();
	}

	_lock.unlock();
}
