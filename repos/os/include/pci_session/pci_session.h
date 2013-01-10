/*
 * \brief  PCI session interface
 * \author Norman Feske
 * \date   2008-01-28
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__PCI_SESSION__PCI_SESSION_H_
#define _INCLUDE__PCI_SESSION__PCI_SESSION_H_

#include <pci_device/pci_device.h>
#include <session/session.h>

namespace Pci {

	typedef Genode::Capability<Device> Device_capability;

	struct Session : Genode::Session
	{
		static const char *service_name() { return "PCI"; }

		virtual ~Session() { }

		/**
		 * Find first accessible device
		 */
		virtual Device_capability first_device() = 0;

		/**
		 * Find next accessible device
		 *
		 * \param prev_device  previous device
		 *
		 * The 'prev_device' argument is used to iterate through all
		 * devices.
		 */
		virtual Device_capability next_device(Device_capability prev_device) = 0;

		/**
		 * Free server-internal data structures representing the device
		 *
		 * Use this function to relax the heap partition of your PCI session.
		 */
		virtual void release_device(Device_capability device) = 0;


		/*********************
		 ** RPC declaration **
		 *********************/

		GENODE_RPC(Rpc_first_device, Device_capability, first_device);
		GENODE_RPC(Rpc_next_device, Device_capability, next_device, Device_capability);
		GENODE_RPC(Rpc_release_device, void, release_device, Device_capability);

		GENODE_RPC_INTERFACE(Rpc_first_device, Rpc_next_device, Rpc_release_device);
	};
}

#endif /* _INCLUDE__PCI_SESSION__PCI_SESSION_H_ */
