/**
 * \brief  Core implementation of the CPU session interface extension
 * \author Alexander Boettcher
 * \date   2012-07-27
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Core includes */
#include <cpu_session_component.h>

using namespace Genode;

void Cpu_session_component::single_step(Thread_capability thread_cap, bool enable)
{
	auto lambda = [&] (Cpu_thread_component *thread) {
		if (!thread) return;

		thread->platform_thread()->single_step(enable);
	};
	_thread_ep->apply(thread_cap, lambda);
}

