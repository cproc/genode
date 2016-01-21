/*
 * \brief  Sampling CPU service
 * \author Christian Prochaska
 * \date   2016-01-15
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <cpu_session/cpu_session.h>
#include <os/attached_dataspace.h>
#include <os/config.h>
#include <os/server.h>
#include <os/session_policy.h>
#include <os/static_root.h>
#include <timer_session/connection.h>
#include <util/list.h>

/* local includes */
#include "cpu_root.h"
#include "cpu_session_component.h"
#include "thread_data.h"
#include "thread_list_change_handler.h"

namespace Sampling_cpu_service
{
	struct Main;
}

static constexpr bool verbose = false;


/******************
 ** Main program **
 ******************/

struct Sampling_cpu_service::Main : Thread_list_change_handler
{
	Server::Entrypoint &ep;
	Allocator *alloc;
	Cpu_root cpu_root;
	Timer::Connection timer;
	List<List_element<Thread_data>> thread_list;
	List<List_element<Thread_data>> selected_thread_list;

	unsigned int sample_index;
	unsigned int max_sample_index;
	unsigned int timeout_us;


	void handle_timeout(unsigned num)
	{
		for (List_element<Thread_data> *thread_data_element = selected_thread_list.first();
		     thread_data_element;
		     thread_data_element = thread_data_element->next()) {

			Thread_data *thread_data = thread_data_element->object();

			if (verbose)
				PDBG("taking sample of thread %s", thread_data->label());

			thread_data->take_sample();

			if (sample_index == max_sample_index)
				thread_data->flush();
		}

		sample_index++;

		if (sample_index == max_sample_index)
			timer.trigger_once(timeout_us);
	}


	Signal_rpc_member<Main> timeout_dispatcher =
		{ ep, *this, &Main::handle_timeout };


	void handle_config_update(unsigned)
	{
		if (verbose)
			PDBG("handle_config_update()");

		Genode::config()->reload();

		sample_index = 0;

		unsigned int sample_rate = 1000;
		unsigned int sample_time = 10000;

    	try {
        	Genode::config()->xml_node().attribute("sample_interval_ms").value(&sample_rate);
    	} catch (...) {
    		PDBG("no sample interval configured, sampling every 1000ms");
    	}

    	try {
        	Genode::config()->xml_node().attribute("sample_duration_ms").value(&sample_time);
    	} catch (...) {
    		PDBG("no sample duration configured, sampling for 10ms");
    	}

		max_sample_index = (sample_time / sample_rate) - 1;

		timeout_us = sample_rate * 1000;

		thread_list_changed();
	}

	Signal_rpc_member<Main> config_update_dispatcher =
		{ ep, *this, &Main::handle_config_update};


	void thread_list_changed() override
	{
		while (List_element<Thread_data> *thread_data_element = selected_thread_list.first()) {

			if (verbose)
				PDBG("removing thread %s from selection", thread_data_element->object()->label());

			selected_thread_list.remove(thread_data_element);
			destroy(alloc, thread_data_element);
		}

		for (List_element<Thread_data> *thread_data_element = thread_list.first();
		     thread_data_element;
		     thread_data_element = thread_data_element->next()) {

			Thread_data *thread_data = thread_data_element->object();

			if (verbose)
				PDBG("evaluating thread %s", thread_data->label());

			try {

				Session_policy policy(String<128>(thread_data->label()));
				thread_data->reset();
				selected_thread_list.insert(new (alloc) List_element<Thread_data>(thread_data));

				if (verbose)
					PDBG("added thread %s to selection", thread_data->label());

			} catch (Session_policy::No_policy_defined) {

				if (verbose)
					PDBG("no session policy defined for thread %s", thread_data->label());

			}
		}

		if (selected_thread_list.first())
			timer.trigger_periodic(timeout_us);
	}


	/**
	 * Constructor
	 */
	Main(Server::Entrypoint &ep)
	: ep(ep),
	  alloc(env()->heap()),
	  cpu_root(ep, alloc, thread_list, *this)
	{
		/*
		 * Register signal handlers
		 */
		Genode::config()->sigh(config_update_dispatcher);
		timer.sigh(timeout_dispatcher);

		/*
		 * Apply initial configuration
		 */
		handle_config_update(0);

		/*
		 * Announce service
		 */
		Genode::env()->parent()->announce(ep.manage(cpu_root));
	}

};


/************
 ** Server **
 ************/

namespace Server {

	char const *name() { return "sampling_cpu_service_ep"; }

	size_t stack_size() { return 4*1024*sizeof(addr_t); }

	void construct(Entrypoint &ep) { static Sampling_cpu_service::Main inst(ep); }
}
