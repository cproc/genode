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


/******************
 ** Main program **
 ******************/

struct Sampling_cpu_service::Main : Thread_list_change_handler
{
	Server::Entrypoint &ep;
	Cpu_root cpu_root;
	Timer::Connection timer;
	List<Thread_data> thread_list;
	List<Thread_data> selected_thread_list;

	unsigned int sample_index;
	unsigned int max_sample_index;


	void handle_timeout(unsigned)
	{
		if (sample_index > max_sample_index) {
			timer.trigger_once(0);
			return;
		}

		for (Thread_data *thread_data = selected_thread_list.first();
		     thread_data;
		     thread_data = thread_data->next()) {

			thread_data->take_sample();

			if (sample_index == max_sample_index)
				thread_data->flush();
		}

		sample_index++;
	}


	Signal_rpc_member<Main> timeout_dispatcher =
		{ ep, *this, &Main::handle_timeout };


	void rebuild_selected_thread_list()
	{
		while (Thread_data *thread_data = selected_thread_list.first())
			selected_thread_list.remove(thread_data);	

		for (Thread_data *thread_data = thread_list.first();
		     thread_data;
		     thread_data = thread_data->next()) {

			try {

				Session_policy policy(String<128>(thread_data->label()));
				thread_data->reset();
				selected_thread_list.insert(thread_data);
				
			} catch (Session_policy::No_policy_defined) {
				PDBG("no session policy defined");
			}
		}
	}


	void handle_config_update(unsigned)
	{
		Genode::config()->reload();

		sample_index = 0;

		rebuild_selected_thread_list();

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

		timer.trigger_periodic(sample_rate * 1000);
	}


	Signal_rpc_member<Main> config_update_dispatcher =
		{ ep, *this, &Main::handle_config_update};


	void thread_list_changed() override
	{
		rebuild_selected_thread_list();
	}


	/**
	 * Constructor
	 */
	Main(Server::Entrypoint &ep)
	: ep(ep),
	  cpu_root(ep, Genode::env()->heap(), thread_list, *this)
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
