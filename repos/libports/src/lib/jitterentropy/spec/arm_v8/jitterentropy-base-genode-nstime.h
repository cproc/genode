/*
 * \brief  Time backend for jitterentropy library
 * \author Sebastian Sumpf
 * \date   2014-05-29
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _JITTERENTROPY_ARM_V8__BASE_GENODE_NSTIME_H_
#define _JITTERENTROPY_ARM_V8__BASE_GENODE_NSTIME_H_

static inline void jent_get_nstime(__u64 *out)
{
	static uint64_t t;
	static int x = 100000;

	/* cycle counter */
	//asm volatile("mrs %0, pmccntr_el0" : "=r" (t));

	t += x;

	if (x < 101000)
		x += 100;
	else
		x = 100000;

	*out = t;
}

#endif /* _JITTERENTROPY_ARM_V8_BASE_GENODE_NSTIME_H */
