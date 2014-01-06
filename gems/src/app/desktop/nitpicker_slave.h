/*
 * \brief  Desktop environment
 * \author Norman Feske
 * \date   2014-01-06
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NITPICKER_SLAVE_H_
#define _NITPICKER_SLAVE_H_

/* Genode includes */
#include <base/lock.h>
#include <os/slave.h>
#include <nitpicker_session/capability.h>
#include <root/client.h>


class Nitpicker_slave : public Genode::Noncopyable
{
	private:

		class Policy : public Genode::Slave_policy
		{
			private:

				Genode::Root_capability _cap;
				bool                    _announced;
				Genode::Lock mutable    _lock;  /* used to wait for announcement */

			protected:

				char const **_permitted_services() const
				{
					static char const *permitted_services[] = {
						"CAP", "LOG", "SIGNAL", "Timer", "Framebuffer", "Input", 0 };

					return permitted_services;
				};

			public:

				Policy(Genode::Rpc_entrypoint &entrypoint,
				       Genode::Ram_session    &ram)
				:
					Slave_policy("nitpicker", entrypoint, &ram),
					_lock(Genode::Lock::LOCKED)
				{
					configure("<config>"
					          "  <global-keys>"
					          "    <key name=\"KEY_SCROLLLOCK\" operation=\"xray\" />"
					          "    <key name=\"KEY_SYSRQ\"      operation=\"kill\" />"
					          "    <key name=\"KEY_PRINT\"      operation=\"kill\" />"
					          "    <key name=\"KEY_F11\"        operation=\"kill\" />"
					          "    <key name=\"KEY_F12\"        operation=\"xray\" />"
					          "  </global-keys>"
					          "</config>");
				}

				bool announce_service(const char             *service_name,
				                      Genode::Root_capability root,
				                      Genode::Allocator      *,
				                      Genode::Server         *)
				{
					PDBG("annouce %s", service_name);
					if (Genode::strcmp(service_name, "Nitpicker"))
						return false;

					_cap = root;
					_lock.unlock();
					return true;
				}

				Genode::Root_capability root() const
				{
					PDBG("get annouced nitpicker service cap");
					/* make sure the nitpicker service was announced */
					Genode::Lock::Guard guard(_lock);
					PDBG("ok we got it");
					return _cap;
				}
		};

		Genode::size_t   const _ep_stack_size = 4*1024*sizeof(Genode::addr_t);
		Genode::Rpc_entrypoint _ep;
		Policy                 _policy;
		Genode::size_t   const _quota = 1024*1024;
		Genode::Slave          _slave;
		Genode::Root_client    _root;

	public:

		/**
		 * Constructor
		 *
		 * \param ep   entrypoint used for nitpicker child thread
		 * \param ram  RAM session used to allocate the configuration
		 *             dataspace
		 */
		Nitpicker_slave(Genode::Cap_session &cap, Genode::Ram_session &ram)
		:
			_ep(&cap, _ep_stack_size, "nitpicker"),
			_policy(_ep, ram),
			_slave(_ep, _policy, _quota),
			_root(_policy.root())
		{ }

		Nitpicker::Session_capability session(char const *label)
		{
			using namespace Genode;

			enum { ARGBUF_SIZE = 128 };
			char argbuf[ARGBUF_SIZE];
			argbuf[0] = 0;

			/*
			 * Declare ram-quota donation
			 */
			enum { SESSION_METADATA = 20*1024 };
			Arg_string::set_arg(argbuf, sizeof(argbuf), "ram_quota", SESSION_METADATA);

			/*
			 * Set session label
			 */
			Arg_string::set_arg(argbuf, sizeof(argbuf), "label", label);

			Session_capability session_cap = _root.session(argbuf, Affinity());

			return static_cap_cast<Nitpicker::Session>(session_cap);
		}

		/**
		 * Upgrade nitpicker session
		 *
		 * \param ram     RAM session from which to take the quota
		 * \param amount  amount of RAM to upgrade, in bytes
		 */
		void upgrade(Nitpicker::Session_capability session_cap,
		             Genode::Ram_session &ram, Genode::size_t amount)
		{
			using namespace Genode;

			/* do the actual quota transfer */
			ram.transfer_quota(_slave.ram().cap(), amount);

			/* notify nitpicker about upgraded quota */
			enum { ARGBUF_SIZE = 128 };
			char argbuf[ARGBUF_SIZE];
			argbuf[0] = 0;
			Arg_string::set_arg(argbuf, sizeof(argbuf), "ram_quota", amount);
			_root.upgrade(session_cap, argbuf);

		}
};

#endif /* _NITPICKER_SLAVE_H_ */
