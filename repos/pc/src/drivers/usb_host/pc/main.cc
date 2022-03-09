/*
 * \brief  i.MX8 USB-card driver Linux port
 * \author Stefan Kalkowski
 * \date   2021-06-29
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/env.h>

#include <lx_emul/init.h>
#include <lx_emul/usb.h>
#include <lx_kit/env.h>
#include <lx_kit/init.h>
#include <lx_user/io.h>

#include <genode_c_api/usb.h>

using namespace Genode;


static bool _bios_handoff;


extern "C" int inhibit_pci_fixup(char const *name)
{
	if (_bios_handoff)
		return 0;

	char const *handoff = "__pci_fixup_final_quirk_usb_early_handoff";

	size_t length = Genode::min(Genode::strlen(name),
	                            Genode::strlen(handoff));

	return Genode::strcmp(handoff, name, length) == 0;
}


struct Main : private Entrypoint::Io_progress_handler
{
	Env                  & env;
	Signal_handler<Main>   signal_handler { env.ep(), *this,
	                                        &Main::handle_signal };
	Sliced_heap            sliced_heap    { env.ram(), env.rm()  };

	struct Initial_config
	{
		Attached_rom_dataspace _config;

		Io_signal_handler<Initial_config> _sigh;

		void _dummy_handle() { }

		Initial_config(Env &env)
		:
			_config(env, "config"),
			_sigh(env.ep(), *this, &Initial_config::_dummy_handle)
		{
			/*
			 * Defer the startup of the USB driver until the first configuration
			 * becomes available. This is needed in scenarios where the configuration
			 * is dynamically generated and supplied to the USB driver via the
			 * report-ROM service.
			 */

			_config.sigh(_sigh);
			_config.update();

			while (_config.xml().type() != "config") {
				env.ep().wait_and_dispatch_one_io_signal();
				_config.update();
			}
		}

		Xml_node xml() { return _config.xml(); }
	};

	/**
	 * Entrypoint::Io_progress_handler
	 */
	void handle_io_progress() override
	{
		genode_usb_notify_peers();
	}

	void handle_signal()
	{
		lx_user_handle_io();
		Lx_kit::env().scheduler.schedule();
	}

	Main(Env & env) : env(env)
	{
		{
			Initial_config config { env };

			_bios_handoff = config.xml().attribute_value("bios_handoff", true);
		}

		Lx_kit::initialize(env);

		genode_usb_init(genode_env_ptr(env),
		                genode_allocator_ptr(sliced_heap),
		                genode_signal_handler_ptr(signal_handler),
		                &lx_emul_usb_rpc_callbacks);

		lx_emul_start_kernel(nullptr);

		env.ep().register_io_progress_handler(*this);
	}
};


void Component::construct(Env & env)
{
	static Main main(env);
}
