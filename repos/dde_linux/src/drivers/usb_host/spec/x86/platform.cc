/*
 * \brief  PCI device handling
 * \author Sebastian Sumpf
 * \date   2016-04-25
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* Genode includes */
#include <base/allocator.h>
#include <base/entrypoint.h>
#include <base/lock.h>
#include <base/signal.h>
#include <irq_session/client.h>
#include <util/misc_math.h>

#include <lx_emul.h>
#include <platform.h>

#include <lx_kit/env.h>
#include <lx_kit/irq.h>
#include <lx_kit/pci_dev_registry.h>
#include <lx_kit/malloc.h>
#include <lx_kit/mapped_io_mem_range.h>

#include <lx_emul/impl/io.h>
#include <lx_emul/impl/pci_resource.h>


/**
 * Quirks
 */
extern "C" void __pci_fixup_quirk_usb_early_handoff(void *data);


/**
 * List of pci devices from platform driver
 */
class Pci_dev_list
{
	private:

		struct Element : public Lx_kit::List<Element>::Element
		{
			Platform::Device_capability cap;

			Element(Platform::Device_capability cap) : cap(cap) { }
		};

		Lx_kit::List<Element> _pci_caps;

	public:

		Pci_dev_list()
		{
			/*
			 * Obtain first device, the operation may exceed the session quota.
			 * So we use the 'with_upgrade' mechanism.
			 */
			Platform::Device_capability cap =
				Lx::pci()->with_upgrade([&] () {
					return Lx::pci()->first_device(); });

			/*
			 * Iterate over the devices of the platform session.
			 */
			while (cap.valid()) {

				_pci_caps.insert(new (Lx::Malloc::mem()) Element(cap));

				/* try next one. Upgrade session quota on demand.*/
				Lx::pci()->with_upgrade([&] () {
					cap = Lx::pci()->next_device(cap); });
			}
		}

		template <typename FUNC>
		void for_each_pci_device(FUNC const &func)
		{
			Element *elements[256] = { 0 };
			int i = 0;
			for (Element *e = _pci_caps.first(); e; e = e->next()) {
				//func(e->cap);
				elements[i] = e;
				i++;
			}

			for (; i > 0; i--)
				func(elements[i - 1]->cap);
		}
};


Pci_dev_list *pci_dev_list()
{
	static Pci_dev_list _list;
	return &_list;
}


extern "C" int pci_register_driver(struct pci_driver *driver)
{
	driver->driver.name = driver->name;

	pci_device_id const *id_table = driver->id_table;
	if (!id_table)
		return -ENODEV;

	using namespace Genode;

	bool found = false;

	auto lamda = [&] (Platform::Device_capability cap) {

		Platform::Device_client client(cap);

		/* request device ID from platform driver */
		unsigned const class_code = client.class_code();

		/* look if we find the device ID in the driver's 'id_table' */
		pci_device_id const *matching_id = nullptr;
		for (pci_device_id const *id = id_table; id->device; id++) {

			lx_log(DEBUG_PCI,"idclass: %x idclassm: %x devclass %x", id->class_,
			       id->class_mask, class_code);

			/* check for drivers that support any device for a given class */
			if (id->device != (unsigned)PCI_ANY_ID || !id->class_mask)
				continue;

			if ((id->class_ & id->class_mask) == (class_code & id->class_mask)) {
				matching_id = id;
				break;
			}
		}

		/* skip device that is not handled by driver */
		if (!matching_id)
			return false;

#if 1
		static int count = 0;
		
		count++;

		Genode::log("count: ", count);

		//if (count != 5)
			//return false;
#endif

Genode::log("check 1");
		/* create 'pci_dev' struct for matching device */
		Lx::Pci_dev *pci_dev = new (Lx::Malloc::mem()) Lx::Pci_dev(cap, false);
	
Genode::log("check 2");
		/* enable ioremap to work */
		Lx::pci_dev_registry()->insert(pci_dev);
Genode::log("check 3");
		/* register driver at the 'pci_dev' struct */
		pci_dev->dev.driver = &driver->driver;
Genode::log("check 4");
		/*
		 * This quirk handles device handoff from BIOS, since the BIOS may still
		 * access the USB controller after bootup. For this the ext cap register of
		 * the PCI config space is checked
		 */
#if 1
		if (Lx_kit::env().config_rom().xml().attribute_value("bios_handoff", true)) {
			Genode::log("handoff");
			__pci_fixup_quirk_usb_early_handoff(pci_dev);
			Genode::log("handoff finished");
		}
#endif
		pci_dev->enable_bus_master();

//while (count == 3) { Genode::log("8"); }

		/* call probe function of the Linux driver */
		if (driver->probe(pci_dev, matching_id)) {

			/* if the probing failed, revert the creation of 'pci_dev' */
			pci_dev_put(pci_dev);
			return false;
		}

		found = true;

		/* multiple device support continue */
		return true;
	};

	pci_dev_list()->for_each_pci_device(lamda);

	return found ? 0 : -ENODEV;
}


/***********************
 ** linux/interrupt.h **
 ***********************/

int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
                const char *name, void *dev)
{
	for (Lx::Pci_dev *pci_dev =  Lx::pci_dev_registry()->first(); pci_dev; pci_dev = pci_dev->next())
		if (pci_dev->irq == irq) {
			{
				Genode::Irq_session_client is(pci_dev->client().irq(0));
				if ((is.info().type != Genode::Irq_session::Info::MSI)
				    && !flags) return 1;
			}
			Lx::Irq::irq().request_irq(pci_dev->client(), handler, dev);
			return 0;
		}

	return -ENODEV;
}


/*********************************
 ** Platform backend alloc init **
 *********************************/

void backend_alloc_init(Genode::Env &env, Genode::Ram_session &ram,
                        Genode::Allocator &alloc)
{
	Lx::pci_init(env, ram, alloc);
}


extern "C" void module_ehci_hcd_init();
extern "C" void module_ehci_pci_init();
extern "C" void module_ohci_hcd_mod_init();
extern "C" void module_ohci_pci_init();
extern "C" void module_uhci_hcd_init();
extern "C" void module_xhci_hcd_init();
extern "C" void module_xhci_pci_init();

void platform_hcd_init(Services *s)
{
	module_xhci_hcd_init();
	module_xhci_pci_init();

	/* ehci_hcd should always be loaded before uhci_hcd and ohci_hcd, not after */
	module_ehci_hcd_init();
	module_ehci_pci_init();

	module_ohci_hcd_mod_init();
	module_ohci_pci_init();

	module_uhci_hcd_init();
}
