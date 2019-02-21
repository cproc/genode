/*
 * \brief  Interface between kernel and userland
 * \author Reto Buerki
 * \date   2015-02-06
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__SPEC__X86_64__KERNEL__INTERFACE_SUPPORT_H_
#define _INCLUDE__SPEC__X86_64__KERNEL__INTERFACE_SUPPORT_H_

/* Genode includes */
#include <base/stdint.h>

namespace Kernel
{
	typedef Genode::uint64_t Call_arg;
	typedef Genode::uint64_t Call_ret;
	typedef Genode::uint64_t Call_ret_64;
}

#endif /* _INCLUDE__SPEC__X86_64__KERNEL__INTERFACE_SUPPORT_H_ */
