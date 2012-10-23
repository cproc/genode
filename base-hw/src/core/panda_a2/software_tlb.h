/*
 * \brief  Software TLB controls specific for the PandaBoard A2
 * \author Martin Stein
 * \date   2012-04-23
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _SRC__CORE__PANDA_A2__SOFTWARE_TLB_H_
#define _SRC__CORE__PANDA_A2__SOFTWARE_TLB_H_

/* Genode includes */
#include <arm/v7/section_table.h>

/**
 * Software TLB controls
 */
class Software_tlb : public Arm_v7::Section_table { };

#endif /* _SRC__CORE__PANDA_A2__SOFTWARE_TLB_H_ */

