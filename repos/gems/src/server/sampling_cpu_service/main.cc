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
#include <base/attached_rom_dataspace.h>
#include <cpu_session/cpu_session.h>
#include <os/attached_dataspace.h>
#include <os/server.h>
#include <os/session_policy.h>
#include <os/static_root.h>
#include <timer_session/connection.h>
#include <util/list.h>

/* local includes */
#include "cpu_root.h"
#include "cpu_session_component.h"
#include "cpu_thread_component.h"
#include "thread_list_change_handler.h"

namespace Sampling_cpu_service
{
	struct Main;
}

static constexpr bool verbose = true;
static constexpr bool verbose_missed_timeouts = true;


/******************
 ** Main program **
 ******************/

struct Sampling_cpu_service::Main : Thread_list_change_handler
{
	Genode::Env &env;
	Allocator *alloc;
	Cpu_root cpu_root;
	Attached_rom_dataspace config;
	Timer::Connection timer;
	List<List_element<Cpu_thread_component>> thread_list;
	List<List_element<Cpu_thread_component>> selected_thread_list;

	unsigned int sample_index;
	unsigned int max_sample_index;
	unsigned int timeout_us;


	void handle_timeout(unsigned num)
	{
		if (verbose) {

			if ((num > 1) && verbose_missed_timeouts)
				PDBG("missed %u timeouts", num - 1);

			if (sample_index == max_sample_index)
				PDBG("sample duration reached");
		}

		for (List_element<Cpu_thread_component> *cpu_thread_element =
		     selected_thread_list.first();
		     cpu_thread_element;
		     cpu_thread_element = cpu_thread_element->next()) {

			Cpu_thread_component *cpu_thread = cpu_thread_element->object();

#if 0
			if (verbose)
				PDBG("taking sample of thread %s", cpu_thread->label().string());
#endif

			cpu_thread->take_sample();

			if (sample_index == max_sample_index)
				cpu_thread->flush();
		}

		sample_index++;

		if (sample_index == max_sample_index)
			timer.trigger_once(timeout_us);
	}


	Signal_rpc_member<Main> timeout_dispatcher =
		{ env.ep(), *this, &Main::handle_timeout };


	void handle_config_update(unsigned)
	{
		if (verbose)
			PDBG("handle_config_update()");

		config.update();

		sample_index = 0;

		unsigned int sample_rate = 1000;
		unsigned int sample_time = 10000;

    	try {
        	//Genode::config()->xml_node().attribute("sample_interval_ms").value(&sample_rate);
        	sample_rate = config.xml().attribute_value("sample_interval_ms", 1000);
    	} catch (...) {
    		PDBG("no sample interval configured, sampling every 1000ms");
    	}

    	try {
        	//Genode::config()->xml_node().attribute("sample_duration_ms").value(&sample_time);
			sample_time = config.xml().attribute_value("sample_duration_ms", 10000);
    	} catch (...) {
    		PDBG("no sample duration configured, sampling for 10ms");
    	}

		max_sample_index = (sample_time / sample_rate) - 1;

		timeout_us = sample_rate * 1000;

		thread_list_changed();

		timer.trigger_periodic(timeout_us);
	}


	Signal_rpc_member<Main> config_update_dispatcher =
		{ env.ep(), *this, &Main::handle_config_update};


	void thread_list_changed() override
	{

		for (List_element<Cpu_thread_component> *cpu_thread_element =
		     selected_thread_list.first();
		     cpu_thread_element;
		     cpu_thread_element = selected_thread_list.first()) {

			if (verbose)
				PDBG("removing thread %s from selection", cpu_thread_element->object()->label().string());

			selected_thread_list.remove(cpu_thread_element);
			destroy(alloc, cpu_thread_element);
		}

		for (List_element<Cpu_thread_component> *cpu_thread_element =
		     thread_list.first();
		     cpu_thread_element;
		     cpu_thread_element = cpu_thread_element->next()) {

			Cpu_thread_component *cpu_thread = cpu_thread_element->object();

			if (verbose)
				PDBG("evaluating thread %s", cpu_thread->label().string());

			try {

				Session_policy policy(String<128>(cpu_thread->label().string()));
				cpu_thread->reset();
				selected_thread_list.insert(new (alloc) List_element<Cpu_thread_component>(cpu_thread));

				if (verbose)
					PDBG("added thread %s to selection", cpu_thread->label().string());

			} catch (Session_policy::No_policy_defined) {

				if (verbose)
					PDBG("no session policy defined for thread %s", cpu_thread->label().string());

			}
		}
	}


	/**
	 * Constructor
	 */
	Main(Genode::Env &env)
	: env(env),
	  alloc(env.heap()),
	  cpu_root(env.ep(), alloc, thread_list, *this),
	  config(env, "config")
	{
		/*
		 * Register signal handlers
		 */
		config.sigh(config_update_dispatcher);
		timer.sigh(timeout_dispatcher);

		/*
		 * Apply initial configuration
		 */
		handle_config_update(0);

		/*
		 * Announce service
		 */
		env.parent()->announce(env.ep().manage(cpu_root));
	}

};


/***************
 ** Component **
 ***************/

Genode::size_t Component::stack_size() { return 4*1024*sizeof(addr_t); }
void Component::construct(Genode::Env &env) { static Sampling_cpu_service::Main inst(env); }
