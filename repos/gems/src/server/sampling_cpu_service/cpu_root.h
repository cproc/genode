/*
 * \brief  CPU root interface
 * \author Christian Helmuth
 * \date   2006-07-17
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CPU_ROOT_H_
#define _CPU_ROOT_H_

/* Genode includes */
#include <root/component.h>

/* local includes */
#include "cpu_session_component.h"

namespace Sampling_cpu_service {

	class Cpu_root : public Root_component<Cpu_session_component>
	{
		protected:

			Cpu_session_component *_create_session(const char *args)
			{
				Cpu_session_component *cpu_session_component =
					new (md_alloc())
					    Cpu_session_component(args);
				return cpu_session_component;
			}

		public:

			/**
			 * Constructor
			 *
			 * \param session_ep   entry point for managing cpu session objects
			 * \param thread_ep    entry point for managing threads
			 * \param md_alloc     meta data allocator to be used by root component
			 */
			Cpu_root(Server::Entrypoint &session_ep,
					 Allocator          *md_alloc)
			:
				Root_component<Cpu_session_component>(&session_ep.rpc_ep(), md_alloc)
			{ }
	};
}

#endif /* _CPU_ROOT_H_ */
