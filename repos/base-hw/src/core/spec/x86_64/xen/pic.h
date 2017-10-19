/*
 * \brief  Programmable interrupt controller for core
 * \author Reto Buerki
 * \date   2015-04-14
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__SPEC__X86_64__XEN__PIC_H_
#define _CORE__SPEC__X86_64__XEN__PIC_H_

#include <spec/x86_64/pic.h>

namespace Xen
{
	/**
	 * IO advanced programmable interrupt controller
	 */
	using Ioapic = X86_64::Ioapic;

	/**
	 * Programmable interrupt controller for core
	 */
	class Pic;
}

class Xen::Pic
{
	private:

		X86_64::Pic x86_64_pic;

	public:

		enum {
			/*
			 * FIXME: dummy ipi value on non-SMP platform, should be removed
			 *        when SMP is an aspect of CPUs only compiled where
			 *        necessary
			 */
			IPI       = 255,
			NR_OF_IRQ = 256,
		};

		Ioapic ioapic;

		/**
		 * Constructor
		 */
		Pic() { }

		bool take_request(unsigned &irq)
		{
			return x86_64_pic.take_request(irq);
		}

		void finish_request()
		{
			x86_64_pic.finish_request();
		}

		void unmask(unsigned const i, unsigned const cpu_id)
		{
			x86_64_pic.unmask(i, cpu_id);
		}

		void mask(unsigned const i)
		{
			x86_64_pic.mask(i);
		}

		/*
		 * Dummies
		 */
		bool is_ip_interrupt(unsigned, unsigned) { return false; }
		void trigger_ip_interrupt(unsigned) { }
};

#endif /* _CORE__SPEC__X86_64__MUEN__PIC_H_ */
