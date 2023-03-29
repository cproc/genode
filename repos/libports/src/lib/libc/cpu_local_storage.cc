/*
 * \brief  CPU-local storage
 * \author Christian Prochaska
 * \date   2023-03-27
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/thread.h>

/* libc-internal includes */
#include <internal/cpu_local_storage.h>
#include <internal/init.h>

using namespace Genode;


static Genode::Env *_env_ptr;


void Libc::init_cpu_local_storage_support(Genode::Env &env)
{
	_env_ptr = &env;
}


/* Affinity location comparison operators for AVL tree */

static bool operator==(Affinity::Location l1, Affinity::Location l2)
{
	return ((l1.xpos() == l2.xpos()) &&
	        (l1.ypos() == l2.ypos()));
}


static bool operator>=(Affinity::Location l1, Affinity::Location l2)
{
	return ((l1.xpos() > l2.xpos()) ||
	        ((l1.xpos() == l2.xpos()) &&
	         (l1.ypos() >= l2.ypos())));
}


bool Libc::Cpu_local_storage::higher(Libc::Cpu_local_storage *other)
{
	return other->location() >= this->location();
}


Libc::Cpu_local_storage *Libc::Cpu_local_storage::find_by_location(Affinity::Location location)
{
	if (location == this->location())
		return this;

	bool side = location >= this->location();
	Libc::Cpu_local_storage *e = Avl_node<Libc::Cpu_local_storage>::child(side);
	return e ? e->find_by_location(location) : 0;
}


Libc::Cpu_local_storage_registry &Libc::cpu_local_storage_registry()
{
	struct Missing_call_of_init_cpu_local_storage_support : Genode::Exception { };

	if (!_env_ptr)
		throw Missing_call_of_init_cpu_local_storage_support();

	static Libc::Cpu_local_storage_registry inst(*_env_ptr);

	return inst;
}
