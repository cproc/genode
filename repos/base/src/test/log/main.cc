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

extern void func(double d);

extern "C" void wait_for_continue();

void Component::construct(Genode::Env &)
{
wait_for_continue();

	double d = 2;

	if (d < 1) Genode::raw("Component::construct(): < 1");

	func(d);
	Genode::func(d);

	Genode::raw("test finished");
}
