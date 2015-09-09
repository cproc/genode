/*
 * \brief  PCI device component implementation
 * \author Alexander Boettcher
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include "pci_session_component.h"
#include "pci_device_component.h"

Genode::Io_port_session_capability Platform::Device_component::io_port(Genode::uint8_t v_id)
{
	Genode::uint8_t max = sizeof(_io_port_conn) / sizeof(_io_port_conn[0]);
	Genode::uint8_t i = 0, r_id = 0;

	for (Resource res = resource(0); i < max; i++, res = resource(i))
	{
		if (res.type() != Resource::IO)
			continue;

		if (v_id != r_id) {
			r_id ++;
			continue;
		}

		if (_io_port_conn[v_id] != nullptr)
			return _io_port_conn[v_id]->cap();

		try {
			_io_port_conn[v_id] = new (_slab_ioport) Genode::Io_port_connection(res.base(), res.size());
			return _io_port_conn[v_id]->cap();
		} catch (...) {
			return Genode::Io_port_session_capability();
		}
	}

	return Genode::Io_port_session_capability();
}

Genode::Io_mem_session_capability Platform::Device_component::io_mem(Genode::uint8_t const v_id,
                                                                     Genode::Cache_attribute const caching,
                                                                     Genode::addr_t const offset,
                                                                     Genode::size_t const size)
{
	Genode::uint8_t max = sizeof(_io_mem) / sizeof(_io_mem[0]);
	Genode::uint8_t i = 0, r_id = 0;

	for (Resource res = resource(0); i < max; i++, res = resource(i))
	{
		if (res.type() != Resource::MEMORY)
			continue;

		if (v_id != r_id) {
			r_id ++;
			continue;
		}

		Genode::size_t res_size = size;
		if (res_size <= 0 || res_size > res.size())
			res_size = res.size();

		if (offset >= res.size() || offset > res.size() - res_size)
			return Genode::Io_mem_session_capability();

		try {
			bool const wc = caching == Genode::Cache_attribute::WRITE_COMBINED;
			Io_mem * io_mem = new (_slab_iomem) Io_mem(res.base() + offset,
			                                           res_size, wc);
			_io_mem[i].insert(io_mem);
			return io_mem->cap();
		} catch (Genode::Allocator::Out_of_memory) {
			throw Platform::Device::Quota_exceeded();
		} catch (...) {
			return Genode::Io_mem_session_capability();
		}
	}

	return Genode::Io_mem_session_capability();
}

void Platform::Device_component::config_write(unsigned char address,
                                              unsigned value,
                                              Access_size size)
{
	/* white list of ports which we permit to write */
	switch (address) {
		case 0x40 ... 0xff:
			/* allow access to device-specific registers if not used by us */
			if (!_device_config.reg_in_use(&_config_access, address, size))
				break;

			PERR("%x:%x:%x write access to address=%x value=0x%x"
			     " size=0x%x denied - it is used by the platform driver.",
			     _device_config.bus_number(), _device_config.device_number(),
			     _device_config.function_number(), address, value, size);
			return;
		case PCI_CMD_REG: /* COMMAND register - first byte */
			if (size == Access_size::ACCESS_16BIT)
				break;
		case PCI_CMD_REG + 1: /* COMMAND register - second byte */
		case 0xd: /* Latency timer */
			if (size == Access_size::ACCESS_8BIT)
				break;
		default:
			PWRN("%x:%x:%x write access to address=%x value=0x%x "
			     " size=0x%x got dropped", _device_config.bus_number(),
				_device_config.device_number(),
				_device_config.function_number(),
				address, value, size);
			return;
	}

	/* assign device to device_pd */
	if (address == PCI_CMD_REG && value & PCI_CMD_DMA && _session)
		_session->assign_device(this);

	_device_config.write(&_config_access, address, value, size,
	                     _device_config.DONT_TRACK_ACCESS);
}
