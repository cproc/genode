/*
 * \brief  Implementation of the cache operations
 * \author Christian Prochaska
 * \date   2022-07-11
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <linux_syscalls.h>

#include <base/log.h>
#include <cpu/cache.h>
#include <cpu/memory_barrier.h>
#include <util/register.h>


#define SYSTEM_REGISTER(sz, name, reg, ...) \
	struct name : Genode::Register<sz> \
	{ \
		static access_t read() \
		{ \
			access_t v; \
			asm volatile ("mrs %0, " #reg : "=r" (v)); \
			return v; \
		} \
 \
		static void write(access_t const v) { \
			asm volatile ("msr " #reg ", %0" :: "r" (v)); } \
 \
		__VA_ARGS__; \
	};


static Genode::size_t cache_line_size()
{
	SYSTEM_REGISTER(32, Ctr_el0,  ctr_el0,
		struct I_min_line : Bitfield<0,  4> {};
		struct D_min_line : Bitfield<16, 4> {};
	);

	static Genode::size_t cache_line_size = 0;

	if (!cache_line_size) {
		Genode::size_t i = 1 << Ctr_el0::I_min_line::get(Ctr_el0::read());
		Genode::size_t d = 1 << Ctr_el0::D_min_line::get(Ctr_el0::read());
		cache_line_size = Genode::min(i,d) * 4; /* word size is fixed in ARM */
	}
	return cache_line_size;
}


template <typename FUNC>
static inline void cache_maintainance(Genode::addr_t const base,
                                      Genode::size_t const size,
                                      FUNC & func)
{
	Genode::addr_t start     = (Genode::addr_t) base;
	Genode::addr_t const end = base + size;
	for (; start < end; start += cache_line_size()) func(start);
}


void Genode::cache_coherent(Genode::addr_t base, Genode::size_t size)
{
	Genode::memory_barrier();

	auto lambda = [] (addr_t const base) {
		asm volatile("dc cvau, %0" :: "r" (base));
		asm volatile("dsb ish");
		asm volatile("ic ivau, %0" :: "r" (base));
		asm volatile("dsb ish");
		asm volatile("isb");
	};

	cache_maintainance(base, size, lambda);
}


void Genode::cache_clean_invalidate_data(Genode::addr_t, Genode::size_t)
{
	error(__func__, " not implemented for this kernel!");
}


void Genode::cache_invalidate_data(Genode::addr_t, Genode::size_t)
{
	error(__func__, " not implemented for this kernel!");
}
