/*
 * \brief  Testing 'Genode::log()' and LOG session
 * \author Christian Prochaska
 * \date   2012-04-20
 *
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/component.h>
#include <base/log.h>
#include <log_session/connection.h>
#include <base/heap.h>


void Component::construct(Genode::Env &env)
{
	using namespace Genode;

	Heap heap(env.ram(), env.rm());

	for (int i = 0; i < 100; i++) {
		log(i);
		Log_connection *l = new (heap) Log_connection(env, "log");
		l->write("test");
	}
}
