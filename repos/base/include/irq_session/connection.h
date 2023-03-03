/*
 * \brief  Connection to IRQ service
 * \author Norman Feske
 * \date   2008-08-22
 */

/*
 * Copyright (C) 2008-2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__IRQ_SESSION__CONNECTION_H_
#define _INCLUDE__IRQ_SESSION__CONNECTION_H_

#include <irq_session/client.h>
#include <base/connection.h>

namespace Genode { struct Irq_connection; }


struct Genode::Irq_connection : Connection<Irq_session>, Irq_session_client
{
	/**
	 * Constructor
	 *
	 * \param label    physical interrupt number
	 * \param trigger  interrupt trigger (e.g., level/edge)
	 * \param polarity interrupt trigger polarity (e.g., low/high)
	 */
	Irq_connection(Env         &env,
	               Label const &label,
	               Trigger      trigger  = Irq_session::TRIGGER_UNCHANGED,
	               Polarity     polarity = Irq_session::POLARITY_UNCHANGED,
	               addr_t       device_config_phys = 0)
	:
		Connection<Irq_session>(env, label, Ram_quota { 6*1024 },
		                        Args("irq_number=",         label, ", "
		                             "irq_trigger=",        unsigned(trigger),  ", "
		                             "irq_polarity=",       unsigned(polarity), ", "
		                             "device_config_phys=", Hex(device_config_phys))),
		Irq_session_client(cap())
	{ }
};

#endif /* _INCLUDE__IRQ_SESSION__CONNECTION_H_ */
