/*
 * \brief  Black hole component
 * \author Christian Prochaska
 * \date   2021-07-07
 *
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */


/* Genode includes */
#include <base/component.h>
#include <base/heap.h>
#include <base/log.h>
#include <root/component.h>

/* local includes */
#include "audio_out.h"


/***************
 ** Component **
 ***************/

namespace Black_hole { struct Main; }
extern "C" void wait_for_continue();
struct Black_hole::Main
{
	Genode::Env &env;

	Genode::Sliced_heap heap { env.ram(), env.rm() };

	Audio_out::Root audio_out_root { env, heap };

	Main(Genode::Env &env) : env(env)
	{
wait_for_continue();
		env.parent().announce(env.ep().manage(audio_out_root));
	}
};


void Component::construct(Genode::Env &env)
{ static Black_hole::Main inst(env); }
