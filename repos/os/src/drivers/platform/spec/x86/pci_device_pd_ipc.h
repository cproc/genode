/*
 * \brief  IPC interface between pci_drv and pci_device_pd
 * \author Alexander Boettcher
 * \date   2013-02-15
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#pragma once

#include <base/connection.h>
#include <base/rpc_server.h>

#include <io_mem_session/capability.h>

namespace Platform {

	struct Device_pd;
	struct Device_pd_client;
	struct Device_pd_connection;
	struct Device_pd_component;
}

struct Platform::Device_pd : Genode::Session
{
	static const char *service_name() { return "DEVICE_PD"; }

	enum { CAP_QUOTA = 2 };

	typedef Device_pd_client Client;

	GENODE_RPC_THROW(Rpc_attach_dma_mem, void, attach_dma_mem,
	                 GENODE_TYPE_LIST(Genode::Out_of_ram, Genode::Out_of_caps),
	                 Genode::Dataspace_capability);
	GENODE_RPC_THROW(Rpc_assign_pci, void, assign_pci,
	                 GENODE_TYPE_LIST(Genode::Out_of_ram, Genode::Out_of_caps,
	                                  Genode::Region_map::Region_conflict),
	                 Genode::Io_mem_dataspace_capability, Genode::uint16_t);

	GENODE_RPC_INTERFACE(Rpc_attach_dma_mem, Rpc_assign_pci);
};

struct Platform::Device_pd_client : Genode::Rpc_client<Device_pd>
{
	Device_pd_client(Capability<Device_pd> cap)
	: Rpc_client<Device_pd>(cap) { }

	void attach_dma_mem(Genode::Dataspace_capability cap) {
		call<Rpc_attach_dma_mem>(cap); }

	void assign_pci(Genode::Io_mem_dataspace_capability cap,
	                Genode::uint16_t bdf)
	{
		call<Rpc_assign_pci>(cap, bdf);
	}
};

struct Platform::Device_pd_connection : Genode::Connection<Device_pd>, Device_pd_client
{
	enum { RAM_QUOTA = 0UL };

	Device_pd_connection(Genode::Capability<Device_pd> cap)
	:
		Genode::Connection<Device_pd>(cap),
		Device_pd_client(cap)
	{ }
};

struct Platform::Device_pd_component : Genode::Rpc_object<Device_pd,
                                                          Device_pd_component>
{
	Genode::Env        &_env;
	Genode::Region_map &_address_space;

	Device_pd_component(Genode::Region_map &address_space, Genode::Env &env)
	: _env(env), _address_space(address_space) { }

	void attach_dma_mem(Genode::Dataspace_capability);
	void assign_pci(Genode::Io_mem_dataspace_capability, Genode::uint16_t);
};
