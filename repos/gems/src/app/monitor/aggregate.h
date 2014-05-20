/*
 * \brief  Process monitor
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _AGGREGATE_H_
#define _AGGREGATE_H_

/* Genode includes */
#include <os/attached_rom_dataspace.h>

/* local includes */
#include <types.h>
#include <type_registry.h>
#include <resource_registry.h>
#include <instance.h>

namespace Monitor {
	class Aggregate_instance;
	class Alias;
}


/**
 * Aggregate instance
 */
class Monitor::Aggregate_instance
{
	private:

		Genode::Parent      &_parent;
		Genode::Ram_session &_ram_session;
		Genode::Rom_session &_config_rom_session;
		Genode::Rm_session  &_local_rm_session;
		Server::Entrypoint  &_local_ep;
		Allocator           &_alloc;

		/**
		 * Registry loaded with the type to instantiate, defined via the config
		 *
		 * The registry contains just a single type.
		 */
		Type_registry _configured_type_registry;

		Genode::Lazy_volatile_object<Genode::Attached_dataspace> _config_ds;

		/**
		 * Instance of configured resource declaration and its instance
		 */
		Resource_registry                      _outer_resources;
		Registry<Type_arg>                     _configured_type_args;
		Genode::Lazy_volatile_object<Resource> _configured_resource;
		Genode::Lazy_volatile_object<Instance> _configured_instance;

		/**
		 * Handler function, called when the config ROM changes
		 */
		void handle_config(unsigned);

		Signal_rpc_member<Aggregate_instance> dispatcher = {
			*this, &Aggregate_instance::handle_config };

		Signal_context_capability config_sigh = _local_ep.manage(dispatcher);

	public:

		/**
		 * Constructor used to create initial aggregate instance
		 */
		Aggregate_instance(Genode::Parent           &parent,
		                   Genode::Ram_session      &ram_session,
		                   Genode::Rom_session      &config_rom_session,
		                   Genode::Rm_session       &local_rm_session,
		                   Server::Entrypoint       &local_ep,
		                   Allocator                &alloc,
		                   Type_registry_base const &outer_types)
		:
			_parent(parent),
			_ram_session(ram_session),
			_config_rom_session(config_rom_session),
			_local_rm_session(local_rm_session),
			_local_ep(local_ep),
			_alloc(alloc),
			_configured_type_registry(outer_types)
		{
			_config_rom_session.sigh(config_sigh);

			/* turn initial configuration into effect */
			handle_config(0);
		}

		void dump_state() const;
};

#endif /* _AGGREGATE_H_ */
