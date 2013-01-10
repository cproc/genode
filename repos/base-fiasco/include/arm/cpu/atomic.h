/*
 * \brief  Atomic operations for ARM
 * \author Norman Feske
 * \date   2007-04-28
 */

/*
 * Copyright (C) 2007-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__ARM__CPU__ATOMIC_H_
#define _INCLUDE__ARM__CPU__ATOMIC_H_

namespace Genode {

	extern "C" long int
	l4_atomic_cmpxchg(volatile long int*, long int, long int);

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
	inline int cmpxchg(volatile int *dest, int cmp_val, int new_val)
	{
		return l4_atomic_cmpxchg((volatile long int *)dest, cmp_val, new_val);
	}
}

#endif /* _INCLUDE__ARM__CPU__ATOMIC_H_ */
