/*
 * \brief  platform device component
 * \author Norman Feske
 * \date   2008-01-28
 */

/*
 * Copyright (C) 2008-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#pragma once

/* base */
#include <base/rpc_server.h>
#include <io_port_session/connection.h>
#include <io_mem_session/connection.h>
#include <util/list.h>
#include <util/mmio.h>
#include <util/construct_at.h>

/* os */
#include <platform_device/platform_device.h>

/* local */
#include "pci_device_config.h"
#include "irq.h"

namespace Platform { class Device_component; class Session_component; }

class Platform::Device_component : public  Genode::Rpc_object<Platform::Device>,
                                   private Genode::List<Device_component>::Element
{
	private:

		friend class Genode::List<Device_component>;

		/*
		 * Noncopyable
		 */
		Device_component(Device_component const &);
		Device_component &operator = (Device_component const &);

		Genode::Env                 &_env;
		Device_config                _device_config { };
		Genode::addr_t               _config_space;
		Config_access                _config_access;
		Platform::Session_component &_session;
		Irq_session_component       *_irq_session = nullptr;
		unsigned short               _irq_line;
		bool                         _enabled_bus_master { false };

		Genode::Allocator           &_global_heap;

		class Io_mem : public  Genode::Io_mem_connection,
		               private Genode::List<Io_mem>::Element
		{
			private:

				friend class Genode::List<Io_mem>;
				friend class Platform::Device_component;

			public:

				Io_mem (Genode::Env &env, Genode::addr_t base,
				        Genode::size_t size, bool wc)
				: Genode::Io_mem_connection(env, base, size, wc) { }
		};

		enum {
			IO_BLOCK_SIZE = sizeof(Genode::Io_port_connection) *
			                Device::NUM_RESOURCES + 32 + 8 * sizeof(void *),
			IO_MEM_SIZE   = sizeof(Io_mem) *
			                Device::NUM_RESOURCES + 32 + 8 * sizeof(void *),
			PCI_IRQ_LINE  = 0x3c,
			PCI_IRQ_PIN   = 0x3d,

			CAP_MSI_64    = 0x80,
			MSI_ENABLED   = 0x1
		};

		struct Msix_ctrl : Register<16>
		{
			struct Slots  : Bitfield< 0, 10> { };
			struct Fmask  : Bitfield<14, 1> { };
			struct Enable : Bitfield<15, 1> { };
		};

		Genode::Tslab<Genode::Io_port_connection, IO_BLOCK_SIZE> _slab_ioport;
		char _slab_ioport_block_data[IO_BLOCK_SIZE];

		Genode::Tslab<Io_mem, IO_MEM_SIZE> _slab_iomem;
		char _slab_iomem_block_data[IO_MEM_SIZE];

		char _mem_irq_component[sizeof(Irq_session_component)];

		Genode::Io_port_connection *_io_port_conn[Device::NUM_RESOURCES];

		/* list of requested resource chunks per BAR */
		Genode::List<Io_mem> _io_mem[Device::NUM_RESOURCES];

		struct Status : Genode::Register<8> {
			struct Capabilities : Bitfield<4,1> { };

			inline static access_t read(Genode::uint8_t t) { return t; };
		};

		/**
		 * Convenience functions to increase readability of code
		 */
		uint16_t _read_config_16(uint16_t const cap)
		{
			return _device_config.read(_config_access, cap,
			                           Platform::Device::ACCESS_16BIT);
		}

		void _write_config_16(uint16_t const cap, uint16_t const value)
		{
			 _device_config.write(_config_access, cap, value,
			                      Platform::Device::ACCESS_16BIT);
		}

		uint32_t _read_config_32(uint16_t const cap)
		{
			return _device_config.read(_config_access, cap,
			                           Platform::Device::ACCESS_32BIT);
		}

		void _write_config_32(uint16_t const cap, uint32_t const value)
		{
			 _device_config.write(_config_access, cap, value,
			                      Platform::Device::ACCESS_32BIT);
		}

		/**
		 * Read out msi capabilities of the device.
		 */
		Genode::uint16_t _msi_cap()
		{
			enum { CAP_MSI = 0x5 };
			return _lookup_cap(CAP_MSI);
		}

		Genode::uint16_t _msix_cap()
		{
			enum { CAP_MSI_X = 0x11 };
			return _lookup_cap(CAP_MSI_X);
		}

		Genode::uint16_t _lookup_cap(Genode::uint16_t const target_cap)
		{
			enum { PCI_STATUS = 0x6, PCI_CAP_OFFSET = 0x34 };

			Status::access_t status = Status::read(_read_config_16(PCI_STATUS));
			if (!Status::Capabilities::get(status))
				return 0;

			uint8_t cap = _read_config_16(PCI_CAP_OFFSET);

			for (Genode::uint16_t val = 0; cap; cap = val >> 8) {
				val = _read_config_16(cap);
				if ((val & 0xff) != target_cap)
					continue;

				return cap;
			}

			return 0;
		}


		/**
		 * Disable MSI/MSI-X if already enabled.
		 */
		unsigned _configure_irq(unsigned irq, uint16_t const msi_cap,
		                        uint16_t const msix_cap)
		{
			using Genode::uint16_t;
			using Genode::uint8_t;

			uint8_t pin = _device_config.read(_config_access, PCI_IRQ_PIN,
			                                  Platform::Device::ACCESS_8BIT);
			if (!pin)
				return Irq_session_component::INVALID_IRQ;

			/* lookup rewrite information as provided by acpi table */
			uint16_t irq_r = Irq_routing::rewrite(_device_config.bdf(), pin);
			if (irq_r) {
				Genode::log(_device_config, " adjust IRQ as reported by ACPI: ",
				            irq, " -> ", irq_r);

				_irq_line = irq = irq_r;
			}

			if (msi_cap) {
				uint16_t msi = _read_config_16(msi_cap + 2);

				if (msi & MSI_ENABLED)
					/* disable MSI */
					_device_config.write(_config_access, msi_cap + 2,
					                     msi ^ MSI_ENABLED,
					                     Platform::Device::ACCESS_8BIT);
			}

			if (msix_cap) {
				uint16_t msix = _read_config_16(msix_cap + 2);

				if (Msix_ctrl::Enable::get(msix)) {
					Msix_ctrl::Enable::set(msix, 0);

					_write_config_16(msix_cap + 2, msix);
				}
			}

			return irq;
		}


		/**
		 * Disable bus master dma if already enabled.
		 */
		void _disable_bus_master_dma() {

			/*
			 * Disabling a bridge may make the devices behind non-functional,
			 * as we have no driver which will switch it on again
			 */
			if (_device_config.pci_bridge())
				return;

			_device_config.disable_bus_master_dma(_config_access);
		}

		bool _setup_msi(Genode::uint16_t);
		bool _setup_msix(Genode::uint16_t);

		template <typename FUNC>
		void apply_msix_table(Pci::Resource const &lookup,
		                      Genode::addr_t const msix_table_phys,
		                      Genode::size_t const msix_table_size,
		                      FUNC const &fn)
		{
			Genode::uint8_t max = sizeof(_io_mem) / sizeof(_io_mem[0]);
			for (unsigned i = 0; i < max; ++i) {
				Pci::Resource res = _device_config.resource(i);

				if (!res.valid() || !res.mem())
					continue;

				if (res.base() != lookup.base() || res.size() != lookup.size())
					continue;

				for (Io_mem * io_mem = _io_mem[i].first(); io_mem; io_mem = io_mem->next()) {

					Dataspace_client ds_client(io_mem->dataspace());

					if (!(ds_client.phys_addr() <= msix_table_phys &&
					      msix_table_phys + msix_table_size <= ds_client.phys_addr() + ds_client.size()))
						continue;

					Genode::size_t const offset = msix_table_phys - ds_client.phys_addr();

					Attached_dataspace mem_io(_env.rm(), io_mem->dataspace());

					fn(reinterpret_cast<addr_t>(mem_io.local_addr<void>()) + offset);

					return;
				}
			}

			/* requested io_mem not allocated by Pci::Resource - try direct */
			Io_mem io_mem(_env, msix_table_phys, msix_table_size, false);
			Attached_dataspace mem_io(_env.rm(), io_mem.dataspace());
			addr_t const offset = msix_table_phys & 0xfffull;
			addr_t const msix_table = reinterpret_cast<addr_t>(mem_io.local_addr<void>()) + offset;

			fn(msix_table);
		};

	public:

		/**
		 * Constructor for PCI devices
		 */
		Device_component(Genode::Env &env,
		                 Device_config device_config, Genode::addr_t addr,
		                 Config_access &config_access,
		                 Platform::Session_component &session,
		                 Genode::Allocator &md_alloc,
		                 Genode::Allocator &global_heap)
		:
			_env(env),
			_device_config(device_config), _config_space(addr),
			_config_access(config_access),
			_session(session),
			_irq_line(_device_config.read(_config_access, PCI_IRQ_LINE,
			                              Platform::Device::ACCESS_8BIT)),
			_global_heap(global_heap),
			_slab_ioport(&md_alloc, &_slab_ioport_block_data),
			_slab_iomem(&md_alloc, &_slab_iomem_block_data)
		{
			for (unsigned i = 0; i < Device::NUM_RESOURCES; i++) {
				_io_port_conn[i] = nullptr;
			}
		}

		/**
		 * Constructor for non PCI devices
		 */
		Device_component(Genode::Env &env,
		                 Genode::Attached_io_mem_dataspace &pciconf,
		                 Platform::Session_component &session, unsigned irq,
		                 Genode::Allocator &global_heap)
		:
			_env(env),
			_config_space(~0UL),
			_config_access(pciconf),
			_session(session),
			_irq_line(irq),
			_global_heap(global_heap),
			_slab_ioport(nullptr, &_slab_ioport_block_data),
			_slab_iomem(nullptr, &_slab_iomem_block_data)
		{
			for (unsigned i = 0; i < Device::NUM_RESOURCES; i++)
				_io_port_conn[i] = nullptr;
		}


		/**
		 * De-constructor
		 */
		~Device_component()
		{
			if (_irq_session) {
				_env.ep().rpc_ep().dissolve(_irq_session);
				_irq_session->~Irq_session_component();
			}

			for (unsigned i = 0; i < Device::NUM_RESOURCES; i++) {
				if (_io_port_conn[i])
					Genode::destroy(_slab_ioport, _io_port_conn[i]);

				while (Io_mem * io_mem = _io_mem[i].first()) {
					_io_mem[i].remove(io_mem);
					Genode::destroy(_slab_iomem, io_mem);
				}
			}

			if (_device_config.valid() && _enabled_bus_master)
				_disable_bus_master_dma();
		}

		/****************************************
		 ** Methods used solely by pci session **
		 ****************************************/

		Device_config device_config() const { return _device_config; }
		Genode::addr_t config_space() const { return _config_space; }

		/**************************
		 ** PCI-device interface **
		 **************************/

		void bus_address(unsigned char *bus, unsigned char *dev,
		                 unsigned char *fn) override
		{
			*bus = _device_config.bdf().bus();
			*dev = _device_config.bdf().device();
			*fn  = _device_config.bdf().function();
		}

		unsigned short vendor_id() override { return _device_config.vendor_id(); }

		unsigned short device_id() override { return _device_config.device_id(); }

		unsigned class_code() override { return _device_config.class_code(); }

		Resource resource(int resource_id) override
		{
			/* return invalid resource if device is invalid */
			if (!_device_config.valid())
				return Resource(0, 0);

			return _device_config.resource(resource_id).api_resource();
		}

		unsigned config_read(unsigned char address, Access_size size) override
		{
			return _device_config.read(_config_access, address, size,
			                           _device_config.DONT_TRACK_ACCESS);
		}

		void config_write(unsigned char address, unsigned value,
		                  Access_size size) override;

		Genode::Irq_session_capability irq(Genode::uint8_t) override;

		Genode::Io_port_session_capability io_port(Genode::uint8_t) override;

		Genode::Io_mem_session_capability io_mem(Genode::uint8_t,
		                                         Genode::Cache_attribute,
		                                         Genode::addr_t,
		                                         Genode::size_t) override;
};
