/*
 * \brief   Cortex A15 cpu context initialization
 * \author  Stefan Kalkowski
 * \date    2017-10-17
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <util/bit_allocator.h>
#include <base/internal/unmanaged_singleton.h>

#include <spec/cortex_a15/cpu.h>

using Asid_allocator = Genode::Bit_allocator<256>;

static Asid_allocator &alloc() {
	return *unmanaged_singleton<Asid_allocator>(); }


Genode::Cpu::Mmu_context::Mmu_context(addr_t table)
: ttbr0(Ttbr_64bit::Ba::masked((Ttbr_64bit::access_t)table)) {
	Ttbr_64bit::Asid::set(ttbr0, (Genode::uint8_t)alloc().alloc()); }
