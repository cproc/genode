/*
 * \brief  CPU root interface
 * \author Christian Prochaska
 * \date   2016-01-19
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
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
#include "thread_data.h"
#include "thread_list_change_handler.h"

namespace Sampling_cpu_service
{
	using namespace Genode;
	class Cpu_root;
}

class Sampling_cpu_service::Cpu_root : public Root_component<Cpu_session_component>
{
	private:

		Allocator                       *_md_alloc;
		List<List_element<Thread_data>> &_thread_list;
		Thread_list_change_handler      &_thread_list_change_handler;

	protected:

		Cpu_session_component *_create_session(const char *args)
		{
			Cpu_session_component *cpu_session_component =
				new (md_alloc()) Cpu_session_component(_md_alloc,
				                                       _thread_list,
				                                       _thread_list_change_handler,
				                                       args);
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
				 Allocator *md_alloc,
				 List<List_element<Thread_data>> &thread_list,
				 Thread_list_change_handler &thread_list_change_handler)
		: Root_component<Cpu_session_component>(&session_ep.rpc_ep(), md_alloc),
		  _md_alloc(md_alloc),
		  _thread_list(thread_list),
		  _thread_list_change_handler(thread_list_change_handler) { }
};

#endif /* _CPU_ROOT_H_ */
