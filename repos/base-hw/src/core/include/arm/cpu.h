/*
 * \brief  Simple driver for the ARM core
 * \author Martin stein
 * \date   2012-09-11
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__ARM__CPU_H_
#define _INCLUDE__ARM__CPU_H_

/* Genode includes */
#include <util/register.h>
#include <cpu/cpu_state.h>

namespace Arm
{
	using namespace Genode;

	/**
	 * ARM core
	 */
	struct Cpu
	{
		enum {
			TTBCR_N = 0,
			EXCEPTION_ENTRY = 0xffff0000,
			DATA_ACCESS_ALIGNM = 4,
		};

		/**
		 * Cache type register
		 */
		struct Ctr : Register<32>
		{
			/**
			 * Read register value
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c0, c0, 1" : [v]"=r"(v) :: );
				return v;
			}
		};

		/**
		 * System control register
		 */
		struct Sctlr : Register<32>
		{
			struct M : Bitfield<0,1> { };  /* enable MMU */
			struct A : Bitfield<1,1> { };  /* strict data addr. alignment on */
			struct C : Bitfield<2,1> { };  /* enable L1 data cache */
			struct Z : Bitfield<11,1> { }; /* enable program flow prediction */
			struct I : Bitfield<12,1> { }; /* enable L1 instruction-cache */

			struct V : Bitfield<13,1> /* select exception-entry base */
			{
				enum { XFFFF0000 = 1 };
			};

			struct Rr : Bitfield<14,1> /* replacement strategy */
			{
				enum { RANDOM = 0 };
			};

			struct Fi : Bitfield<21,1> { }; /* enable fast IRQ config */

			struct Ve : Bitfield<24,1> /* interrupt vector config */
			{
				enum { FIXED = 0 };
			};

			struct Ee : Bitfield<25,1> { }; /* raise CPSR.E on exceptions */

			/**
			 * Value for the switch to virtual mode in kernel
			 */
			static access_t init_virt_kernel()
			{
				return M::bits(1) |
				       A::bits(0) |
				       C::bits(0) |
				       Z::bits(0) |
				       I::bits(0) |
				       V::bits(V::XFFFF0000) |
				       Rr::bits(Rr::RANDOM) |
				       Fi::bits(0) |
				       Ve::bits(Ve::FIXED) |
				       Ee::bits(0);
			}

			/**
			 * Value for the initial kernel entry
			 */
			static access_t init_phys_kernel()
			{
				return M::bits(0) |
				       A::bits(0) |
				       C::bits(0) |
				       Z::bits(0) |
				       I::bits(0) |
				       V::bits(V::XFFFF0000) |
				       Rr::bits(Rr::RANDOM) |
				       Fi::bits(0) |
				       Ve::bits(Ve::FIXED) |
				       Ee::bits(0);
			}

			/**
			 * Read register value
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c1, c0, 0" : [v]"=r"(v) :: );
				return v;
			}

			/**
			 * Write register value
			 */
			static void write(access_t const v)
			{
				asm volatile ("mcr p15, 0, %[v], c1, c0, 0" :: [v]"r"(v) : );
			}
		};

		/**
		 * Translation table base control register
		 */
		struct Ttbcr : Register<32>
		{
			struct N : Bitfield<0, 3> { }; /* base address width */

			/**
			 * Write register, only in privileged CPU mode
			 */
			static void write(access_t const v) {
				asm volatile ("mcr p15, 0, %[v], c2, c0, 2" :: [v]"r"(v) : ); }

			/**
			 * Read register value
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c2, c0, 2" : [v]"=r"(v) :: );
				return v;
			}

			/**
			 * Value for the switch to virtual mode in kernel
			 */
			static access_t init_virt_kernel() { return N::bits(TTBCR_N); }
		};

		/**
		 * Translation table base register 0
		 */
		struct Ttbr0 : Register<32>
		{
			struct Irgn_1 : Bitfield<0,1> /* inner cachable mode */
			{
				enum { NON_CACHEABLE = 0 };
			};

			struct S : Bitfield<1,1> { }; /* shareable */

			struct Rgn : Bitfield<3, 2> /* outer cachable attributes */
			{
				enum { NON_CACHEABLE = 0 };
			};

			struct Ba : Bitfield<14-TTBCR_N, 18+TTBCR_N> { }; /* translation
			                                                   * table base */

			/**
			 * Write register, only in privileged CPU mode
			 */
			static void write(access_t const v) {
				asm volatile ("mcr p15, 0, %[v], c2, c0, 0" :: [v]"r"(v) : ); }

			/**
			 * Read register value
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c2, c0, 0" : [v]"=r"(v) :: );
				return v;
			}

			/**
			 * Value for the switch to virtual mode in kernel
			 *
			 * \param sect_table  pointer to initial section table
			 */
			static access_t init_virt_kernel(addr_t const sect_table)
			{
				return S::bits(0) |
				       Irgn_1::bits(Irgn_1::NON_CACHEABLE) |
				       Rgn::bits(Rgn::NON_CACHEABLE) |
				       Ba::masked((addr_t)sect_table);
			}
		};

		/**
		 * Domain access control register
		 */
		struct Dacr : Register<32>
		{
			enum Dx_values { NO_ACCESS = 0, CLIENT = 1 };

			/**
			 * Access values for the 16 available domains
			 */
			struct D0  : Bitfield<0,2> { };
			struct D1  : Bitfield<2,2> { };
			struct D2  : Bitfield<4,2> { };
			struct D3  : Bitfield<6,2> { };
			struct D4  : Bitfield<8,2> { };
			struct D5  : Bitfield<10,2> { };
			struct D6  : Bitfield<12,2> { };
			struct D7  : Bitfield<14,2> { };
			struct D8  : Bitfield<16,2> { };
			struct D9  : Bitfield<18,2> { };
			struct D10 : Bitfield<20,2> { };
			struct D11 : Bitfield<22,2> { };
			struct D12 : Bitfield<24,2> { };
			struct D13 : Bitfield<26,2> { };
			struct D14 : Bitfield<28,2> { };
			struct D15 : Bitfield<30,2> { };

			/**
			 * Write register, only in privileged CPU mode
			 */
			static void write(access_t const v) {
				asm volatile ("mcr p15, 0, %[v], c3, c0, 0" :: [v]"r"(v) : ); }

			/**
			 * Initialize for Genodes operational mode
			 */
			static access_t init_virt_kernel()
			{
				return D0::bits(CLIENT)     | D1::bits(NO_ACCESS)  |
				       D2::bits(NO_ACCESS)  | D3::bits(NO_ACCESS)  |
				       D4::bits(NO_ACCESS)  | D5::bits(NO_ACCESS)  |
				       D6::bits(NO_ACCESS)  | D7::bits(NO_ACCESS)  |
				       D8::bits(NO_ACCESS)  | D9::bits(NO_ACCESS)  |
				       D10::bits(NO_ACCESS) | D11::bits(NO_ACCESS) |
				       D12::bits(NO_ACCESS) | D13::bits(NO_ACCESS) |
				       D14::bits(NO_ACCESS) | D15::bits(NO_ACCESS);
			}
		};

		/**
		 * Context identification register
		 */
		struct Cidr : Register<32>
		{
			/**
			 * Write register value
			 */
			static void write(access_t const v)
			{
				asm volatile ("mcr p15, 0, %[v], c13, c0, 1" :: [v]"r"(v) : );
			}

			/**
			 * Read register value
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c13, c0, 1" : [v]"=r"(v) :: );
				return v;
			}
		};

		/**
		 * Program status register
		 */
		struct Psr : Register<32>
		{
			struct M : Bitfield<0,5> /* processor mode */
			{
				enum { USER = 0b10000, SUPERVISOR = 0b10011 };
			};

			struct T : Bitfield<5,1> /* instruction state */
			{
				enum { ARM = 0 };
			};

			struct F : Bitfield<6,1> { }; /* FIQ disable */
			struct I : Bitfield<7,1> { }; /* IRQ disable */
			struct A : Bitfield<8,1> { }; /* asynchronous abort disable */

			struct E : Bitfield<9,1> /* load/store endianess */
			{
				enum { LITTLE = 0 };
			};

			struct J : Bitfield<24,1> /* instruction state */
			{
				enum { ARM = 0 };
			};

			/**
			 * Read register
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrs %[v], cpsr" : [v] "=r" (v) : : );
				return v;
			}

			/**
			 * Write register
			 */
			static void write(access_t const v) {
				asm volatile ("msr cpsr, %[v]" : : [v] "r" (v) : ); }

			/**
			 * Initial value for a user execution context with trustzone
			 *
			 * FIXME: This function should not be declared in 'Arm' but in
			 *        'Arm_v7', but for now the declaration is necessary
			 *        because of 'User_context::User_context()'.
			 */
			inline static access_t init_user_with_trustzone();

			/**
			 * Initial value for an userland execution context
			 */
			static access_t init_user()
			{
				return M::bits(M::USER) |
				       T::bits(T::ARM) |
				       F::bits(1) |
				       I::bits(0) |
				       A::bits(1) |
				       E::bits(E::LITTLE) |
				       J::bits(J::ARM);
			}

			/**
			 * Initial value for the kernel execution context
			 */
			static access_t init_kernel()
			{
				return M::bits(M::SUPERVISOR) |
				       T::bits(T::ARM) |
				       F::bits(1) |
				       I::bits(1) |
				       A::bits(1) |
				       E::bits(E::LITTLE) |
				       J::bits(J::ARM);
			}
		};

		/**
		 * Common parts of fault status registers
		 */
		struct Fsr : Register<32>
		{
			/**
			 * Fault status encoding
			 */
			enum Fault_status
			{
				SECTION_TRANSLATION = 5,
				PAGE_TRANSLATION = 7,
			};

			struct Fs_3_0 : Bitfield<0, 4> { };  /* fault status */
			struct Fs_4   : Bitfield<10, 1> { }; /* fault status */
		};

		/**
		 * Instruction fault status register
		 */
		struct Ifsr : Fsr
		{
			/**
			 * Read register value
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c5, c0, 1" : [v]"=r"(v) :: );
				return v;
			}

			/**
			 * Read fault status
			 */
			static Fault_status fault_status()
			{
				access_t const v = read();
				return (Fault_status)(Fs_3_0::get(v) |
				                     (Fs_4::get(v) << Fs_3_0::WIDTH));
			}
		};

		/**
		 * Data fault status register
		 */
		struct Dfsr : Fsr
		{
			struct Wnr : Bitfield<11, 1> { }; /* write not read bit */

			/**
			 * Read register value
			 */
			static access_t read()
			{
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c5, c0, 0" : [v]"=r"(v) :: );
				return v;
			}

			/**
			 * Read fault status
			 */
			static Fault_status fault_status() {
				access_t const v = read();
				return (Fault_status)(Fs_3_0::get(v) |
				                     (Fs_4::get(v) << Fs_3_0::WIDTH));
			}
		};

		/**
		 * Data fault address register
		 */
		struct Dfar : Register<32>
		{
			/**
			 * Read register value
			 */
			static access_t read() {
				access_t v;
				asm volatile ("mrc p15, 0, %[v], c6, c0, 0" : [v]"=r"(v) :: );
				return v;
			}
		};

		/**
		 * Extend basic CPU state by members relevant for 'base-hw' only
		 */
		struct Context : Cpu_state
		{
			/**********************************************************
			 ** The offset and width of any of these classmembers is **
			 ** silently expected to be this way by several assembly **
			 ** files. So take care if you attempt to change them.   **
			 **********************************************************/

			uint32_t cidr;          /* context ID register backup */
			uint32_t section_table; /* base address of applied section table */

			/***************
			 ** Accessors **
			 ***************/

			void software_tlb(addr_t const st) { section_table = st; }

			addr_t software_tlb() const { return section_table; }

			void protection_domain(unsigned const id) { cidr = id; }
		};

		/**
		 * An usermode execution state
		 */
		struct User_context : Context
		{
			/**
			 * Constructor
			 */
			User_context();

			/***************************************************
			 ** Communication between user and context holder **
			 ***************************************************/

			void user_arg_0(unsigned const arg) { r0 = arg; }
			void user_arg_1(unsigned const arg) { r1 = arg; }
			void user_arg_2(unsigned const arg) { r2 = arg; }
			void user_arg_3(unsigned const arg) { r3 = arg; }
			void user_arg_4(unsigned const arg) { r4 = arg; }
			void user_arg_5(unsigned const arg) { r5 = arg; }
			void user_arg_6(unsigned const arg) { r6 = arg; }
			void user_arg_7(unsigned const arg) { r7 = arg; }
			unsigned user_arg_0() const { return r0; }
			unsigned user_arg_1() const { return r1; }
			unsigned user_arg_2() const { return r2; }
			unsigned user_arg_3() const { return r3; }
			unsigned user_arg_4() const { return r4; }
			unsigned user_arg_5() const { return r5; }
			unsigned user_arg_6() const { return r6; }
			unsigned user_arg_7() const { return r7; }

			/**
			 * Read a general purpose register
			 *
			 * \param   id  ID of the targeted register
			 * \param   v   Holds register value if this returns 1
			 */
			bool get_gpr(unsigned id, unsigned & v) const
			{
				switch(id)
				{
				case 0:  v = r0;  return 1;
				case 1:  v = r1;  return 1;
				case 2:  v = r2;  return 1;
				case 3:  v = r3;  return 1;
				case 4:  v = r4;  return 1;
				case 5:  v = r5;  return 1;
				case 6:  v = r6;  return 1;
				case 7:  v = r7;  return 1;
				case 8:  v = r8;  return 1;
				case 9:  v = r9;  return 1;
				case 10: v = r10; return 1;
				case 11: v = r11; return 1;
				case 12: v = r12; return 1;
				case 13: v = sp;  return 1;
				case 14: v = lr;  return 1;
				case 15: v = ip;  return 1;
				}
				return 0;
			}

			/**
			 * Override a general purpose register
			 *
			 * \param   id  ID of the targeted register
			 * \param   v   Has been written to register if this returns 1
			 */
			bool set_gpr(unsigned id, unsigned const v)
			{
				switch(id)
				{
				case 0:  r0 = v;  return 1;
				case 1:  r1 = v;  return 1;
				case 2:  r2 = v;  return 1;
				case 3:  r3 = v;  return 1;
				case 4:  r4 = v;  return 1;
				case 5:  r5 = v;  return 1;
				case 6:  r6 = v;  return 1;
				case 7:  r7 = v;  return 1;
				case 8:  r8 = v;  return 1;
				case 9:  r9 = v;  return 1;
				case 10: r10 = v; return 1;
				case 11: r11 = v; return 1;
				case 12: r12 = v; return 1;
				case 13: sp = v;  return 1;
				case 14: lr = v;  return 1;
				case 15: ip = v;  return 1;
				}
				return 0;
			}

			/**
			 * Check if a pagefault has occured due to a translation miss
			 *
			 * \param va  holds the virtual fault-address if this returns 1
			 * \param w   wether it is a write fault if this returns 1
			 */
			bool translation_miss(addr_t & va, bool & w) const
			{
				/* determine fault type */
				switch (cpu_exception) {

				case PREFETCH_ABORT: {

					/* check if fault was caused by a translation miss */
					Ifsr::Fault_status const fs = Ifsr::fault_status();
					if (fs == Ifsr::SECTION_TRANSLATION ||
					    fs == Ifsr::PAGE_TRANSLATION)
					{
						/* fetch fault data */
						w = 0;
						va = ip;
						return 1;
					}
					return 0; }

				case DATA_ABORT: {

					/* check if fault was caused by translation miss */
					Dfsr::Fault_status const fs = Dfsr::fault_status();
					if(fs == Dfsr::SECTION_TRANSLATION ||
					   fs == Dfsr::PAGE_TRANSLATION)
					{
						/* fetch fault data */
						Dfsr::access_t const dfsr = Dfsr::read();
						w = Dfsr::Wnr::get(dfsr);
						va = Dfar::read();
						return 1;
					}
					return 0; }

				default: return 0;
				}
			}
		};

		/**
		 * Flush all instruction caches
		 */
		__attribute__((always_inline)) static void flush_instr_caches() {
			asm volatile ("mcr p15, 0, %[rd], c7, c5, 0" :: [rd]"r"(0) : ); }

		/**
		 * Flush all data caches
		 */
		inline static void flush_data_caches();

		/**
		 * Flush all caches
		 */
		static void flush_caches()
		{
			flush_data_caches();
			flush_instr_caches();
		}

		/**
		 * Invalidate all TLB entries of one address space
		 *
		 * \param pid  ID of the targeted address space
		 */
		static void flush_tlb_by_pid(unsigned const pid)
		{
			asm volatile ("mcr p15, 0, %[pid], c8, c7, 2" :: [pid]"r"(pid) : );
			flush_caches();
		}

		/**
		 * Invalidate all TLB entries
		 */
		static void flush_tlb()
		{
			asm volatile ("mcr p15, 0, %[rd], c8, c7, 0" :: [rd]"r"(0) : );
			flush_caches();
		}
	};
}

#endif /* _INCLUDE__ARM__CPU_H_ */

