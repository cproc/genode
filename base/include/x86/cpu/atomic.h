/*
 * \brief  Atomic operations for x86
 * \author Norman Feske
 * \date   2006-07-26
 *
 * Based on l4util/include/ARCH-x86/atomic_arch.h.
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__X86__CPU__ATOMIC_H_
#define _INCLUDE__X86__CPU__ATOMIC_H_

namespace Genode {

	/**
	 * Atomic compare and exchange
	 *
	 * This function compares the value at dest with cmp_val.
	 * If both values are equal, dest is set to new_val. If
	 * both values are different, the value at dest remains
	 * unchanged.
	 *
	 * \return  1 if the value was successfully changed to new_val,
	 *          0 if cmp_val and the value at dest differ.
	 */
	template <typename T>
	inline int cmpxchg(volatile T *dest, T cmp_val, T new_val)
	{
	    return __sync_bool_compare_and_swap(dest, cmp_val, new_val);
	}
}

#endif /* _INCLUDE__X86__CPU__ATOMIC_H_ */
