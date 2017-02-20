/*
 * \brief  Platform interface
 * \author Martin Stein
 * \author Stefan Kalkowski
 * \date   2011-12-21
 */

/*
 * Copyright (C) 2011-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__INCLUDE__PLATFORM_H_
#define _CORE__INCLUDE__PLATFORM_H_

/* Genode includes */
#include <base/synced_allocator.h>
#include <base/allocator_avl.h>
#include <irq_session/irq_session.h>

/* base-hw includes */
#include <kernel/configuration.h>
#include <kernel/core_interface.h>

/* core includes */
#include <bootinfo.h>
#include <translation_table_allocator_tpl.h>
#include <platform_generic.h>
#include <core_region_map.h>
#include <core_mem_alloc.h>
#include <memory_region.h>

namespace Genode {

	/**
	 * Manages all platform ressources
	 */
	class Platform : public Platform_generic
	{
		private:

			Core_mem_allocator _core_mem_alloc; /* core-accessible memory */
			Phys_allocator     _io_mem_alloc;   /* MMIO allocator         */
			Phys_allocator     _io_port_alloc;  /* I/O port allocator     */
			Phys_allocator     _irq_alloc;      /* IRQ allocator          */
			Rom_fs             _rom_fs;         /* ROM file system        */

			static Genode::Bootinfo const &            _bootinfo();
			static Genode::Memory_region_array const & _core_virt_regions();

			/**
			 * Initialize I/O port allocator
			 */
			 void _init_io_port_alloc();

			/**
			 * Initialize IO memory allocator
			 *
			 * Use byte granularity for MMIO regions because on some platforms,
			 * devices driven by core share a physical page with devices
			 * driven outside of core. Using byte granularity allows handing
			 * out the MMIO page to trusted user-level device drivers.
			 */
			 void _init_io_mem_alloc();

			 /**
			  * Perform additional platform-specific initialization.
			  */
			 void _init_additional();

		public:

			Platform();

			static addr_t mmio_to_virt(addr_t mmio);

			/**
			 * Return platform IRQ-number for user IRQ-number 'user_irq'
			 */
			static long irq(long const user_irq);

			/**
			 * Setup mode of an IRQ to specified trigger mode and polarity
			 *
			 * \param irq_number  ID of targeted interrupt
			 * \param trigger     new interrupt trigger mode
			 * \param polarity    new interrupt polarity setting
			 */
			static void setup_irq_mode(unsigned irq_number, unsigned trigger,
			                           unsigned polarity);

			/**
			 * Get MSI-related parameters from device PCI config space
			 *
			 * \param mmconf      PCI config space address of device
			 * \param address     MSI address register value to use
			 * \param data        MSI data register value to use
			 * \param irq_number  IRQ to use
			 *
			 * \return  true if the device is MSI-capable, false if not
			 */
			static bool get_msi_params(const addr_t mmconf,
			                           addr_t &address, addr_t &data,
			                           unsigned &irq_number);

			static addr_t core_phys_addr(addr_t virt);

			static Translation_table * core_translation_table();

			static Translation_table_allocator * core_translation_table_allocator();


			/********************************
			 ** Platform_generic interface **
			 ********************************/

			Range_allocator * core_mem_alloc() {
				return &_core_mem_alloc; }

			Range_allocator * ram_alloc() {
				return _core_mem_alloc.phys_alloc(); }

			Range_allocator * region_alloc() {
				return _core_mem_alloc.virt_alloc(); }

			Range_allocator * io_mem_alloc() { return &_io_mem_alloc; }

			Range_allocator * io_port_alloc() { return &_io_port_alloc; }

			Range_allocator * irq_alloc() { return &_irq_alloc; }

			addr_t vm_start() const { return VIRT_ADDR_SPACE_START; }

			size_t vm_size() const { return VIRT_ADDR_SPACE_SIZE; }

			Rom_fs *rom_fs() { return &_rom_fs; }

			inline void wait_for_exit() {
				while (1) { Kernel::stop_thread(); } };

			bool supports_direct_unmap() const { return 1; }

			Affinity::Space affinity_space() const {
				return Affinity::Space(NR_OF_CPUS); }
	};
}

#endif /* _CORE__INCLUDE__PLATFORM_H_ */
