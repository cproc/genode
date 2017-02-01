/*
 * \brief  Slave used for toggling the visibility of a nitpicker session
 * \author Norman Feske
 * \date   2014-09-30
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NIT_FADER_SLAVE_H_
#define _NIT_FADER_SLAVE_H_

/* Genode includes */
#include <os/static_parent_services.h>
#include <os/slave.h>
#include <nitpicker_session/nitpicker_session.h>

/* local includes */
#include <types.h>

namespace Launcher { class Nit_fader_slave; }


class Launcher::Nit_fader_slave
{
	private:

		class Policy
		:
			private Genode::Static_parent_services<Genode::Ram_session,
			                                       Genode::Cpu_session,
			                                       Genode::Pd_session,
			                                       Genode::Rom_session,
			                                       Genode::Log_session,
			                                       Timer::Session>,
			public Slave::Policy
		{
			private:

				Genode::Service &_nitpicker_service;

			protected:

				static Name   _name()  { return "nit_fader"; }
				static size_t _quota() { return 2*1024*1024; }

			public:

				Policy(Rpc_entrypoint         &ep,
				       Region_map             &rm,
				       Ram_session_capability  ram,
				       Genode::Service        &nitpicker_service)
				:
					Genode::Slave::Policy(_name(), _name(), *this, ep, rm, ram, _quota()),
					_nitpicker_service(nitpicker_service)
				{
					visible(false);
				}

				void visible(bool visible)
				{
					char config[256];
					snprintf(config, sizeof(config),
					         "<config alpha=\"%d\" />", visible ? 255 : 0);
					configure(config, strlen(config) + 1);
				}

				Genode::Service &resolve_session_request(Genode::Service::Name const &service,
				                                         Genode::Session_state::Args const &args) override
				{
					if (service == Nitpicker::Session::service_name())
						return _nitpicker_service;

					return Genode::Slave::Policy::resolve_session_request(service, args);
				}
		};

		Policy _policy;
		Child  _child;

	public:

		/**
		 * Constructor
		 *
		 * \param ep   entrypoint used for nitpicker child thread
		 * \param ram  RAM session used to allocate the configuration
		 *             dataspace
		 */
		Nit_fader_slave(Rpc_entrypoint        &ep,
		                Genode::Region_map    &rm,
		                Ram_session_capability ram,
		                Genode::Service       &nitpicker_service)
		:
			_policy(ep, rm, ram, nitpicker_service),
			_child(rm, ep, _policy)
		{
			visible(false);
		}

		Genode::Slave::Policy &policy() { return _policy; }

		void visible(bool visible)
		{
			_policy.visible(visible);
		}
};

#endif /* _NIT_FADER_SLAVE_H_ */
