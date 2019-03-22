/*
 * \brief   Parts of platform that are specific to Imx7 sabrelite
 * \author  Stefan Kalkowski
 * \date    2018-11-07
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <platform.h>
#include <spec/arm/imx_aipstz.h>

extern "C" void *    _start_setup_stack;   /* entrypoint for non-boot CPUs */
static unsigned char hyp_mode_stack[1024]; /* hypervisor mode's kernel stack */

using namespace Board;

Bootstrap::Platform::Board::Board()
: early_ram_regions(Memory_region { RAM_0_BASE, RAM_0_SIZE }),
  core_mmio(Memory_region { IRQ_CONTROLLER_BASE, IRQ_CONTROLLER_SIZE },
            Memory_region { UART_1_MMIO_BASE, UART_1_MMIO_SIZE })
{
	Aipstz aipstz_1(AIPS_1_MMIO_BASE);
	Aipstz aipstz_2(AIPS_2_MMIO_BASE);
	Aipstz aipstz_3(AIPS_3_MMIO_BASE);

	/* configure CSU */
	for (addr_t start = 0x303e0000; start <= 0x303e00fc; start += 4)
		*(volatile addr_t *)start = 0x00ff00ff;

	static volatile unsigned long initial_values[][2] {
		// CCM (Clock Control Module)
		{ 0x30384000, 0x3 },
		{ 0x30384040, 0x3 },
		{ 0x30384060, 0x3 },
		{ 0x30384130, 0x3 },
		{ 0x30384160, 0x0 },
		{ 0x303844f0, 0x3 },
		{ 0x30384510, 0x0 },
		{ 0x30384520, 0x3 },
		{ 0x303846d0, 0x0 },
		{ 0x303846e0, 0x0 },
		{ 0x30384780, 0x0 },
		{ 0x30384790, 0x0 },
		{ 0x303847a0, 0x0 },
		{ 0x303847b0, 0x0 },
		{ 0x303847c0, 0x3 },
		{ 0x30384880, 0x0 },
		{ 0x303848a0, 0x0 },
		{ 0x30384950, 0x0 },
		{ 0x30384960, 0x0 },
		{ 0x30384970, 0x0 },
		{ 0x30384980, 0x0 },
		{ 0x30384990, 0x0 },
		{ 0x303849a0, 0x0 },
		{ 0x303849d0, 0x0 },
		{ 0x303849e0, 0x0 },
		{ 0x303849f0, 0x0 },
		{ 0x303600c0, 0xd2605a56 },
		{ 0x303600d0, 0xd2d2d256 },
		{ 0x303600d4, 0xd2d2d256 },
		{ 0x303600d8, 0xd2d2d256 },
		{ 0x303600dc, 0xd2d2d256 },
		{ 0x303600e0, 0x80000600 },
		{ 0x303600f0, 0x101b },
		// IOMUXC (IOMUX Controller)
		{ 0x30330030, 0x14 },
		{ 0x30330034, 0x10 },
		{ 0x30330074, 0x2 },
		{ 0x30330078, 0x2 },
		{ 0x3033007c, 0x2 },
		{ 0x30330080, 0x2 },
		{ 0x30330084, 0x2 },
		{ 0x30330088, 0x2 },
		{ 0x3033008c, 0x2 },
		{ 0x30330090, 0x2 },
		{ 0x30330094, 0x2 },
		{ 0x30330098, 0x2 },
		{ 0x3033009c, 0x2 },
		{ 0x303300a0, 0x2 },
		{ 0x303300c4, 0x0 },
		{ 0x30330150, 0x10 },
		{ 0x30330154, 0x10 },
		{ 0x30330210, 0x13 },
		{ 0x30330214, 0x13 },
		{ 0x3033021c, 0x1 },
		{ 0x30330220, 0x1 },
		{ 0x30330224, 0x1 },
		{ 0x303302e4, 0x1 },
		{ 0x303302e8, 0x1 },
		{ 0x303302ec, 0x1 },
		{ 0x303302f0, 0x1 },
		{ 0x303302f4, 0x1 },
		{ 0x303302f8, 0x1 },
		{ 0x303302fc, 0x1 },
		{ 0x30330300, 0x1 },
		{ 0x30330304, 0x1 },
		{ 0x30330308, 0x1 },
		{ 0x3033030c, 0x1 },
		{ 0x30330310, 0x1 },
		{ 0x30330318, 0x59 },
		{ 0x303303c0, 0x7f },
		{ 0x303303c4, 0x7f },
		{ 0x303303f4, 0x34 },
		{ 0x303303f8, 0x59 },
		{ 0x303303fc, 0x59 },
		{ 0x30330400, 0x59 },
		{ 0x30330404, 0x19 },
		{ 0x30330408, 0x59 },
		{ 0x3033040c, 0x59 },
		{ 0x30330410, 0x59 },
		{ 0x30330414, 0x59 },
		{ 0x30330418, 0x59 },
		{ 0x3033041c, 0x59 },
		{ 0x30330440, 0x19 },
		{ 0x30330444, 0x59 },
		{ 0x30330448, 0x59 },
		{ 0x3033044c, 0x59 },
		{ 0x30330450, 0x59 },
		{ 0x30330454, 0x59 },
		{ 0x30330458, 0x59 },
		{ 0x3033045c, 0x59 },
		{ 0x30330460, 0x59 },
		{ 0x30330464, 0x59 },
		{ 0x30330468, 0x19 },
		{ 0x30330480, 0x7f },
		{ 0x30330484, 0x7f },
		{ 0x3033048c, 0x2 },
		{ 0x30330490, 0x2 },
		{ 0x30330494, 0x2 },
		{ 0x3033049c, 0x1 },
		{ 0x303304a0, 0x1 },
		{ 0x303304a4, 0x1 },
		{ 0x303304a8, 0x1 },
		{ 0x303304ac, 0x1 },
		{ 0x303304b0, 0x1 },
		{ 0x303304b4, 0x1 },
		{ 0x303304b8, 0x1 },
		{ 0x303304bc, 0x1 },
		{ 0x303304c0, 0x1 },
		{ 0x303304c4, 0x1 },
		{ 0x303304c8, 0x1 },
		{ 0x30330544, 0x1 },
		{ 0x30330548, 0x1 },
		{ 0x3033054c, 0x1 },
		{ 0x303305dc, 0x1 },
		{ 0x303305e0, 0x1 },
		{ 0x303305ec, 0x3 },
		{ 0x303305f0, 0x3 }
 	};

	unsigned num_values = sizeof(initial_values) / (2*sizeof(unsigned long));
	for (unsigned i = 0; i < num_values; i++)
		*((volatile unsigned long*)initial_values[i][0]) = initial_values[i][1];

}


static inline void prepare_nonsecure_world(unsigned long timer_freq)
{
	using Cpu = Hw::Arm_cpu;

	/* if we are already in HYP mode we're done (depends on u-boot version) */
	if (Cpu::Psr::M::get(Cpu::Cpsr::read()) == Cpu::Psr::M::HYP)
		return;

	/* ARM generic timer counter freq needs to be set in secure mode */
	Cpu::Cntfrq::write(timer_freq);

	/*
	 * enable coprocessor 10 + 11 access and SMP bit access in auxiliary control
	 * register for non-secure world
	 */
	Cpu::Nsacr::access_t nsacr = 0;
	Cpu::Nsacr::Cpnsae10::set(nsacr, 1);
	Cpu::Nsacr::Cpnsae11::set(nsacr, 1);
	Cpu::Nsacr::Ns_smp::set(nsacr, 1);
	Cpu::Nsacr::write(nsacr);

	asm volatile (
		"msr sp_mon, sp \n" /* copy current mode's sp */
		"msr lr_mon, lr \n" /* copy current mode's lr */
		"cps #22        \n" /* switch to monitor mode */
		);

	Cpu::Scr::access_t scr = 0;
	Cpu::Scr::Ns::set(scr,  1);
	Cpu::Scr::Fw::set(scr,  1);
	Cpu::Scr::Aw::set(scr,  1);
	Cpu::Scr::Scd::set(scr, 1);
	Cpu::Scr::Hce::set(scr, 1);
	Cpu::Scr::Sif::set(scr, 1);
	Cpu::Scr::write(scr);
}


static inline void prepare_hypervisor(Genode::addr_t table)
{
	using Cpu = Hw::Arm_cpu;

	/* set hypervisor exception vector */
	Cpu::Hvbar::write(Hw::Mm::hypervisor_exception_vector().base);

	/* set hypervisor's translation table */
	Cpu::Httbr_64bit::write(table);

	Cpu::Ttbcr::access_t ttbcr = 0;
	Cpu::Ttbcr::Irgn0::set(ttbcr, 1);
	Cpu::Ttbcr::Orgn0::set(ttbcr, 1);
	Cpu::Ttbcr::Sh0::set(ttbcr, 2);
	Cpu::Ttbcr::Eae::set(ttbcr, 1);

	/* prepare MMU usage by hypervisor code */
	Cpu::Htcr::write(ttbcr);

	/* don't trap on cporocessor 10 + 11, but all others */
	Cpu::Hcptr::access_t hcptr = 0;
	Cpu::Hcptr::Tcp<0>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<1>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<2>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<3>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<4>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<5>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<6>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<7>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<8>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<9>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<12>::set(hcptr, 1);
	Cpu::Hcptr::Tcp<13>::set(hcptr, 1);
	Cpu::Hcptr::Tta::set(hcptr, 1);
	Cpu::Hcptr::Tcpac::set(hcptr, 1);
	Cpu::Hcptr::write(hcptr);

	enum Memory_attributes {
		DEVICE_MEMORY          = 0x04,
		NORMAL_MEMORY_UNCACHED = 0x44,
		NORMAL_MEMORY_CACHED   = 0xff,
	};

	Cpu::Mair0::access_t mair0 = 0;
	Cpu::Mair0::Attr0::set(mair0, NORMAL_MEMORY_UNCACHED);
	Cpu::Mair0::Attr1::set(mair0, DEVICE_MEMORY);
	Cpu::Mair0::Attr2::set(mair0, NORMAL_MEMORY_CACHED);
	Cpu::Mair0::Attr3::set(mair0, DEVICE_MEMORY);
	Cpu::Hmair0::write(mair0);

	Cpu::Vtcr::access_t vtcr = ttbcr;
	Cpu::Vtcr::Sl0::set(vtcr, 1); /* set to starting level 1 */
	Cpu::Vtcr::write(vtcr);

	Cpu::Sctlr::access_t sctlr = Cpu::Sctlr::read();
	Cpu::Sctlr::C::set(sctlr, 1);
	Cpu::Sctlr::I::set(sctlr, 1);
	Cpu::Sctlr::V::set(sctlr, 1);
	Cpu::Sctlr::A::set(sctlr, 0);
	Cpu::Sctlr::M::set(sctlr, 1);
	Cpu::Sctlr::Z::set(sctlr, 1);
	Cpu::Hsctlr::write(sctlr);
}


static inline void switch_to_supervisor_mode()
{
	using Cpsr = Hw::Arm_cpu::Psr;

	Cpsr::access_t cpsr = 0;
	Cpsr::M::set(cpsr, Cpsr::M::SVC);
	Cpsr::F::set(cpsr, 1);
	Cpsr::I::set(cpsr, 1);

	asm volatile (
		"msr sp_svc, sp        \n" /* copy current mode's sp           */
		"msr lr_svc, lr        \n" /* copy current mode's lr           */
		"msr elr_hyp, lr       \n" /* copy current mode's lr to hyp lr */
		"msr sp_hyp, %[stack]  \n" /* copy to hyp stack pointer        */
		"msr spsr_cxfs, %[cpsr] \n" /* set psr for supervisor mode      */
		"adr lr, 1f            \n" /* load exception return address    */
		"eret                  \n" /* exception return                 */
		"1:":: [cpsr] "r" (cpsr), [stack] "r" (&hyp_mode_stack));
}


unsigned Bootstrap::Platform::enable_mmu()
{
	static volatile bool primary_cpu = true;
	static unsigned long timer_freq  = Cpu::Cntfrq::read();
	pic.init_cpu_local();

	prepare_nonsecure_world(timer_freq);
	prepare_hypervisor((addr_t)core_pd->table_base);
	switch_to_supervisor_mode();

	Cpu::Sctlr::init();
	Cpu::Cpsr::init();

	/* primary cpu wakes up all others */
	if (primary_cpu && NR_OF_CPUS > 1) {
		cpu.invalidate_data_cache();
		primary_cpu = false;
		cpu.wake_up_all_cpus(&_start_setup_stack);
	}

	cpu.enable_mmu_and_caches((Genode::addr_t)core_pd->table_base);

	return Cpu::Mpidr::Aff_0::get(Cpu::Mpidr::read());
}


void Bootstrap::Cpu::wake_up_all_cpus(void * const ip)
{
	struct Src : Genode::Mmio
	{
		struct A7_cr0 : Register<0x4,  32>
		{
			struct Core1_por_reset  : Bitfield<1,1> {};
			struct Core1_soft_reset : Bitfield<5,1> {};
		};
		struct A7_cr1 : Register<0x8,  32>
		{
			struct Core1_enable : Bitfield<1,1> {};
		};
		struct Gpr3 : Register<0x7c, 32> {}; /* ep core 1 */
		struct Gpr4 : Register<0x80, 32> {}; /* ep core 1 */

		Src(void * const entry) : Genode::Mmio(SRC_MMIO_BASE)
		{
			write<Gpr3>((Gpr3::access_t)entry);
			write<Gpr4>((Gpr4::access_t)entry);
			A7_cr0::access_t v0 = read<A7_cr0>();
			A7_cr0::Core1_soft_reset::set(v0,1);
			write<A7_cr0>(v0);
			A7_cr1::access_t v1 = read<A7_cr1>();
			A7_cr1::Core1_enable::set(v1,1);
			write<A7_cr1>(v1);
		}
	};

	Src src(ip);
}
