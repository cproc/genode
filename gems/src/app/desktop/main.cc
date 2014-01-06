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

/* Genode includes */
#include <os/server.h>
#include <nitpicker_session/client.h>
#include <nitpicker_view/client.h>
#include <framebuffer_session/client.h>
#include <cap_session/connection.h>

/* local includes */
#include <nitpicker_slave.h>

namespace Desktop { class Main;
	using Genode::size_t;
	using Genode::env;
}


class Decorator
{
	private:

		Nitpicker_slave &_nitpicker_slave;

		Genode::Ram_session &_ram;

		Nitpicker::Session_client _nitpicker_session {

			/* XXX we might leave the label empty for the decorator session */
			_nitpicker_slave.session("decorator")
		};

		Framebuffer::Mode _mode = { _nitpicker_session.mode() };

		Framebuffer::Session_capability _init_framebuffer()
		{
			bool const use_alpha = false;

			_nitpicker_slave.upgrade(_nitpicker_session, _ram,
			                         Nitpicker::Session::ram_quota(_mode, use_alpha));

			_nitpicker_session.buffer(_mode, use_alpha);

			return _nitpicker_session.framebuffer_session();
		}

		Framebuffer::Session_client _framebuffer { _init_framebuffer() };

		struct Local_framebuffer_service : Genode::Service
		{
			Framebuffer::Session_capability fb_cap;

			Local_framebuffer_service(Framebuffer::Session_capability fb_cap)
			: Service("Framebuffer"), fb_cap(fb_cap) { }

			Genode::Session_capability
			session(const char *, Genode::Affinity const &) override {
				return fb_cap; }

			void upgrade(Genode::Session_capability, const char *) override { }
			void close(Genode::Session_capability) { }
		};

		class Policy : public Genode::Slave_policy
		{
			private:

				Local_framebuffer_service _local_fb_service;

			protected:

				char const **_permitted_services() const
				{
					static char const *permitted_services[] = {
						"CAP", "LOG", "SIGNAL", 0 };

					return permitted_services;
				};

			public:

				Policy(Genode::Rpc_entrypoint         &entrypoint,
				       Genode::Ram_session            &ram,
				       Framebuffer::Session_capability fb_cap)
				:
					Slave_policy("decorator", entrypoint, &ram),
					_local_fb_service(fb_cap)
				{
					configure("<config>"
					          "  <scene>"
					          "    <box xpos=\"0\" ypos=\"0\" width=\"400\" height=\"400\" color=\"#101215\" />"
					
					          "    <box xpos=\"100\" ypos=\"50\" width=\"200\" height=\"99\" color=\"#771111\" />"
					          "    <box xpos=\"100\" ypos=\"50\" width=\"200\" height=\"1\"  color=\"#994444\" />"
					
					          "    <label xpos=\"50\" ypos=\"70\" color=\"#ffffff\""
					          "           text=\"Bright text with no clipping\"/>"
					
					          "    <box xpos=\"100\" ypos=\"150\" width=\"200\" height=\"99\" color=\"#115511\" />"
					          "    <box xpos=\"100\" ypos=\"150\" width=\"200\" height=\"1\"  color=\"#448844\" />"
					
					          "    <box xpos=\"100\" ypos=\"250\" width=\"200\" height=\"99\" color=\"#222255\" >"
					          "      <label xpos=\"110\" ypos=\"262\" color=\"#bbbbdd\""
					          "             text=\"A dark string that goes beyond the clipping boundary\"/>"
					          "    </box>"
					          "    <box xpos=\"100\" ypos=\"250\" width=\"200\" height=\"1\"  color=\"#4444aa\" />"
					          "  </scene>"
					          "</config>");
				}

				Genode::Service *resolve_session_request(const char *service_name,
				                                         const char *args) override
				{
					if (Genode::strcmp(service_name, "Framebuffer") == 0) {
						PDBG("Framebuffer requested");
						return &_local_fb_service;
					}

					return Genode::Slave_policy::resolve_session_request(service_name, args);
				}
		};

		Genode::size_t   const _ep_stack_size = 4*1024*sizeof(Genode::addr_t);
		Genode::Rpc_entrypoint _ep;
		Policy                 _policy;
		Genode::size_t   const _quota = 1024*1024;
		Genode::Slave          _slave;

	public:

		class Window
		{
		};

		/**
		 * Constructor
		 *
		 * \param ram  RAM session for paying the nitpicker session
		 */
		Decorator(Genode::Cap_session &cap,
		          Nitpicker_slave     &nitpicker_slave,
		          Genode::Ram_session &ram)
		:
			_nitpicker_slave(nitpicker_slave),
			_ram(ram),
			_ep(&cap, _ep_stack_size, "decorator"),
			_policy(_ep, ram, _nitpicker_session.framebuffer_session()),
			_slave(_ep, _policy, _quota)
		{
			PDBG("decorator mode is %dx%d", _mode.width(), _mode.height());

			Nitpicker::View_client view(_nitpicker_session.create_view());

			PDBG("created view");

			view.viewport(0, 0,
			              _mode.width()/2,
			              _mode.height(), 0, 0, true);
			view.stack(Nitpicker::View_capability(), true, true);
			PDBG("configured view");
		}
};


struct Desktop::Main
{
	Server::Entrypoint ep;

	Genode::Cap_connection cap;

	Nitpicker_slave nitpicker_slave = { cap, *env()->ram_session() };

	Decorator decorator = { cap, nitpicker_slave, *env()->ram_session() };

	Main(Server::Entrypoint &ep) : ep(ep) { }
};


/************
 ** Server **
 ************/

namespace Server {

	char const *name() { return "desktop_ep"; }

	size_t stack_size() { return 4*1024*sizeof(long); }

	void construct(Entrypoint &ep)
	{
		static Desktop::Main desktop(ep);
		PDBG("finished construction");
	}
}
