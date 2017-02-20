/*
 * \brief  Programmable interrupt controller for core
 * \author Stefan Kalkowski
 * \date   2012-10-24
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* core includes */
#include <pic.h>
#include <platform.h>

using namespace Genode;

Pic::Pic() : Mmio(Platform::mmio_to_virt(Board::IRQ_CONTROLLER_BASE)) { }

void Pic::_init_security_ext() { }

void Pic::unsecure(unsigned) { }

void Pic::secure(unsigned) { }
