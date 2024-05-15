/*
 * \brief  SVM specific implementations
 * \author Benjamin Lamowski
 * \date   2022-10-14
 */

/*
 * Copyright (C) 2022-2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/log.h>
#include <hw/spec/x86_64/x86_64.h>
#include <kernel/cpu.h>
#include <platform.h>
#include <spec/x86_64/virtualization/svm.h>
#include <util/mmio.h>

using namespace Genode;

using Kernel::Cpu;
using Kernel::Vm;
using Board::Vmcb;


Vmcb::Vmcb(uint32_t id)
:
	Mmio({(char *)this, Mmio::SIZE})
{
	write<Guest_asid>(id);
	write<Msrpm_base_pa>(dummy_msrpm());
	write<Iopm_base_pa>(dummy_iopm());

	/*
	 * Set the guest PAT register to the default value.
	 * See: AMD Vol.2 7.8 Page-Attribute Table Mechanism
	 */
	g_pat = 0x0007040600070406ULL;
}


Vmcb & Vmcb::host_vmcb(size_t cpu_id)
{
	static Constructible<Vmcb> host_vmcb[NR_OF_CPUS];

	if (!host_vmcb[cpu_id].constructed()) {
		host_vmcb[cpu_id].construct(Vmcb::Asid_host);
	}
	return *host_vmcb[cpu_id];
}

void Vmcb::initialize(Kernel::Cpu &cpu, addr_t page_table_phys_addr)
{
	using Cpu = Hw::X86_64_cpu;

	Cpu::Ia32_efer::access_t ia32_efer_msr = Cpu::Ia32_efer::read();
	Cpu::Ia32_efer::Svme::set(ia32_efer_msr, 1);
	Cpu::Ia32_efer::write(ia32_efer_msr);

	Cpu::Amd_vm_syscvg::access_t amd_vm_syscvg_msr =
	    Cpu::Amd_vm_syscvg::read();
	Cpu::Amd_vm_syscvg::Nested_paging::set(amd_vm_syscvg_msr, 1);
	Cpu::Amd_vm_syscvg::write(amd_vm_syscvg_msr);

	root_vmcb_phys =
	    Core::Platform::core_phys_addr((addr_t)&host_vmcb(cpu.id()));
	asm volatile ("vmsave" : : "a" (root_vmcb_phys) : "memory");
	Cpu::Amd_vm_hsavepa::write((Cpu::Amd_vm_hsavepa::access_t) root_vmcb_phys);

	/*
	 * enable nested paging
	 */
	write<Npt_control::Np_enable>(1);
	write<N_cr3>(page_table_phys_addr);

	write<Int_control::V_intr_mask>(1); /* See 15.2 */
	write<Intercept_ex::Vectors>(17);   /* AC */

	enforce_intercepts();
}


/*
 * Enforce SVM intercepts
 */
void Vmcb::enforce_intercepts(uint32_t desired_primary, uint32_t desired_secondary)
{
	write<Vmcb::Intercept_misc1>(
		desired_primary                           |
		Vmcb::Intercept_misc1::Intr::bits(1)      |
		Vmcb::Intercept_misc1::Nmi::bits(1)       |
		Vmcb::Intercept_misc1::Init::bits(1)      |
		Vmcb::Intercept_misc1::Invd::bits(1)      |
		Vmcb::Intercept_misc1::Hlt::bits(1)       |
		Vmcb::Intercept_misc1::Ioio_prot::bits(1) |
		Vmcb::Intercept_misc1::Msr_prot::bits(1)  |
		Vmcb::Intercept_misc1::Shutdown::bits(1)
	);
	write<Vmcb::Intercept_misc2>(
		desired_secondary                      |
		Vmcb::Intercept_misc2::Vmload::bits(1) |
		Vmcb::Intercept_misc2::Vmsave::bits(1) |
		Vmcb::Intercept_misc2::Clgi::bits(1)   |
		Vmcb::Intercept_misc2::Skinit::bits(1)
	);
}


/*
 * AMD Vol.2 15.11: MSR Permissions Map
 * All set to 1 since we want all MSRs to be intercepted.
 */
addr_t Vmcb::dummy_msrpm()
{
	static Board::Msrpm msrpm;

	return Core::Platform::core_phys_addr((addr_t) &msrpm);
}


/*
 * AMD Vol.2 15.10.1 I/O Permissions Map
 * All set to 1 since we want all IO port accesses to be intercepted.
 */
addr_t Vmcb::dummy_iopm()
{
	static Board::Iopm iopm;

	return Core::Platform::core_phys_addr((addr_t) &iopm);
}


Board::Msrpm::Msrpm()
{
	memset(this, 0xFF, sizeof(*this));
}


Board::Iopm::Iopm()
{
	memset(this, 0xFF, sizeof(*this));
}


void Vmcb::write_vcpu_state(Vcpu_state &state)
{
	typedef Vcpu_state::Range Range;

	state.ax.charge(rax);
	state.ip.charge(rip);
	/*
	 * SVM doesn't use ip_len, so just leave the old value.
	 * We still have to charge it when charging ip.
	 */
	state.ip_len.set_charged();

	state.flags.charge(rflags);
	state.sp.charge(rsp);

	state.dr7.charge(dr7);

	state.cr0.charge(cr0);
	state.cr2.charge(cr2);
	state.cr3.charge(cr3);
	state.cr4.charge(cr4);

	state.cs.charge(cs);
	state.ss.charge(ss);
	state.es.charge(es);
	state.ds.charge(ds);
	state.fs.charge(fs);
	state.gs.charge(gs);
	state.tr.charge(tr);
	state.ldtr.charge(ldtr);
	state.gdtr.charge(Range { .limit = gdtr.limit, .base = gdtr.base });

	state.idtr.charge(Range { .limit = idtr.limit, .base = idtr.base });

	state.sysenter_cs.charge(sysenter_cs);
	state.sysenter_sp.charge(sysenter_esp);
	state.sysenter_ip.charge(sysenter_eip);

	state.qual_primary.charge(read<Vmcb::Exitinfo1>());
	state.qual_secondary.charge(read<Vmcb::Exitinfo2>());

	/* Charging ctrl_primary and ctrl_secondary breaks Virtualbox 6 */

	state.inj_info.charge(read<Vmcb::Exitintinfo>() & 0xFFFFFFFF);
	state.inj_error.charge(
	    (uint32_t)(read<Vmcb::Exitintinfo>() >> 32));

	/* Guest is in an interrupt shadow, see 15.21.5 */
	state.intr_state.charge(
	    (unsigned)read<Vmcb::Int_control_ext::Int_shadow>());
	/* Guest activity state (actv) not used by SVM */
	state.actv_state.set_charged();

	state.tsc.charge(Hw::Lapic::rdtsc());
	state.tsc_offset.charge(read<Vmcb::Tsc_offset>());

	state.efer.charge(efer);

	/* pdpte not used by SVM */

	state.star.charge(star);
	state.lstar.charge(lstar);
	state.cstar.charge(cstar);
	state.fmask.charge(sfmask);
	state.kernel_gs_base.charge(kernel_gs_base);

	/* Task Priority Register, see 15.24 */
	state.tpr.charge((unsigned)read<Vmcb::Int_control::V_tpr>());
	/* TPR threshold not used by SVM */
}


void Vmcb::read_vcpu_state(Vcpu_state &state)
{
	if (state.ax.charged())       rax = state.ax.value();
	if (state.flags.charged()) rflags = state.flags.value();
	if (state.sp.charged())       rsp = state.sp.value();
	if (state.ip.charged())       rip = state.ip.value();
	/* ip_len not used by SVM */
	if (state.dr7.charged()) dr7 = state.dr7.value();

	if (state.cr0.charged()) cr0 =  state.cr0.value();
	if (state.cr2.charged()) cr2 =  state.cr2.value();
	if (state.cr3.charged()) cr3 =  state.cr3.value();
	if (state.cr4.charged()) cr4 =  state.cr4.value();

	if (state.cs.charged()) cs =  state.cs.value();
	if (state.ss.charged()) ss =  state.ss.value();

	if (state.es.charged()) es =  state.es.value();
	if (state.ds.charged()) ds =  state.ds.value();

	if (state.fs.charged()) fs =  state.fs.value();
	if (state.gs.charged()) gs =  state.gs.value();

	if (state.tr.charged())     tr =  state.tr.value();
	if (state.ldtr.charged()) ldtr =  state.ldtr.value();

	if (state.gdtr.charged()) {
		gdtr.limit = state.gdtr.value().limit;
		gdtr.base  = state.gdtr.value().base;
	}

	if (state.idtr.charged()) {
		idtr.limit = state.idtr.value().limit;
		idtr.base  = state.idtr.value().base;
	}

	if (state.sysenter_cs.charged()) sysenter_cs  = state.sysenter_cs.value();
	if (state.sysenter_sp.charged()) sysenter_esp = state.sysenter_sp.value();
	if (state.sysenter_ip.charged()) sysenter_eip = state.sysenter_ip.value();

	if (state.ctrl_primary.charged() || state.ctrl_secondary.charged()) {
		enforce_intercepts(state.ctrl_primary.value(),
			           state.ctrl_secondary.value());
	}

	if (state.inj_info.charged() || state.inj_error.charged()) {
		/* Honor special signaling bit */
		if (state.inj_info.value() & 0x1000) {
			write<Vmcb::Int_control::V_irq>(1);
			write<Vmcb::Int_control::V_ign_tpr>(1);
			write<Vmcb::Intercept_misc1::Vintr>(1);
		} else {
			write<Vmcb::Int_control::V_irq>(0);
			write<Vmcb::Int_control::V_ign_tpr>(0);
			write<Vmcb::Intercept_misc1::Vintr>(0);
		}

		write<Vmcb::Eventinj>(
			/* Filter out special signaling bits */
			(state.inj_info.value()     &
			(uint32_t) ~0x3000) |
			(((uint64_t) state.inj_error.value()) << 32)
		);
	}

	if (state.intr_state.charged()) {
		write<Vmcb::Int_control_ext::Int_shadow>(
		    state.intr_state.value());
	}
	/* Guest activity state (actv) not used by SVM */

	if (state.tsc_offset.charged()) {
		/* state.tsc not used by SVM */
		write<Vmcb::Tsc_offset>(read<Vmcb::Tsc_offset>() +
			                state.tsc_offset.value());
	}

	if (state.efer.charged()) {
		efer = state.efer.value();
	}

	/* pdpte not used by SVM */

	if (state.star.charged()) star                     = state.star.value();
	if (state.cstar.charged()) cstar                   = state.cstar.value();
	if (state.lstar.charged()) lstar                   = state.lstar.value();
	if (state.fmask.charged()) sfmask                  = state.fmask.value();
	if (state.kernel_gs_base.charged()) kernel_gs_base = state.kernel_gs_base.value();

	if (state.tpr.charged()) {
		write<Vmcb::Int_control::V_tpr>(state.tpr.value());
		/* TPR threshold not used on AMD */
	}
}

uint64_t Vmcb::get_exitcode()
{
	enum Svm_exitcodes : uint64_t
	{
		VMEXIT_INVALID = -1ULL,
		VMEXIT_INTR    =  0x60,
		VMEXIT_NPF     = 0x400,
	};

	uint64_t exitcode = read<Vmcb::Exitcode>();
	switch (exitcode) {
		case VMEXIT_INVALID:
			error("Vm::exception: invalid SVM state!");
			break;
		case 0x40 ... 0x5f:
			error("Vm::exception: unhandled SVM exception ",
			              Hex(exitcode));
			break;
		case VMEXIT_INTR:
			exitcode = EXIT_PAUSED;
			break;
		case VMEXIT_NPF:
			exitcode = EXIT_NPF;
			break;
		default:
			break;
	}

	return exitcode;
}

void Vmcb::switch_world(addr_t vmcb_phys_addr, Core::Cpu::Context &regs)
{
	/*
	 * We push the host context's physical address to trapno so that
	 * we can pop it later
	 */
	regs.trapno = root_vmcb_phys;
	asm volatile(
	    "fxrstor (%[fpu_context]);"
	    "mov %[guest_state], %%rax;"
	    "mov  %[regs], %%rsp;"
	    "popq %%r8;"
	    "popq %%r9;"
	    "popq %%r10;"
	    "popq %%r11;"
	    "popq %%r12;"
	    "popq %%r13;"
	    "popq %%r14;"
	    "popq %%r15;"
	    "add $8, %%rsp;" /* don't pop rax */
	    "popq %%rbx;"
	    "popq %%rcx;"
	    "popq %%rdx;"
	    "popq %%rdi;"
	    "popq %%rsi;"
	    "popq %%rbp;"
	    "clgi;"
	    "sti;"
	    "vmload;"
	    "vmrun;"
	    "vmsave;"
	    "popq %%rax;" /* get the physical address of the host VMCB from
	                     the stack */
	    "vmload;"
	    "stgi;" /* maybe enter the kernel to handle an external interrupt
	               that occured ... */
	    "nop;"
	    "cli;"        /* ... otherwise, just disable interrupts again */
	    "pushq $256;" /* make the stack point to trapno, the right place
	                     to jump to _kernel_entry. We push 256 because
	                     this is outside of the valid range for interrupts
	                   */
	    "jmp _kernel_entry;" /* jump to _kernel_entry to save the
	                            GPRs without breaking any */
	    :
	    : [regs] "r"(&regs.r8), [fpu_context] "r"(regs.fpu_context()),
	      [guest_state] "r"(vmcb_phys_addr)
	    : "rax", "memory");
}
