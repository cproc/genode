/*
 * \brief  Genode specific VirtualBox SUPLib supplements.
 *         File used by Genode platforms not supporting hardware
 *         virtualisation features.
 * \author Alexander Boettcher
 * \date   2013-11-18
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <base/semaphore.h>
#include <os/attached_io_mem_dataspace.h>

/* VirtualBox includes */
#include "HMInternal.h" /* enable access to hm.s.* */
#include "CPUMInternal.h" /* enable access to cpum.s.* */
#include <VBox/vmm/vm.h>
#include <VBox/vmm/rem.h>
#include <VBox/vmm/pdmapi.h>
#include <VBox/err.h>

/* Genode's VirtualBox includes */
#include "sup.h"
#include "vcpu.h"
#include "vm_handler.h"
#include "vmm_memory.h"

/* Libc include */
#include <pthread.h>

enum {
	VMCS_SEG_UNUSABLE  = 0x10000,
	BLOCKING_BY_STI    = 1U << 0,
	BLOCKING_BY_MOV_SS = 1U << 1,
};

#define GENODE_READ_SELREG_REQUIRED(REG) \
    (pCtx->REG.Sel != cur_state->REG.sel) || \
    (pCtx->REG.ValidSel != cur_state->REG.sel) || \
    (pCtx->REG.fFlags   != CPUMSELREG_FLAGS_VALID) || \
    (pCtx->REG.u32Limit != cur_state->REG.limit) || \
    (pCtx->REG.u64Base  != cur_state->REG.base) || \
    (pCtx->REG.Attr.u   != cur_state->REG.access)

#define GENODE_READ_SELREG(REG) \
	pCtx->REG.Sel      = cur_state->REG.sel; \
	pCtx->REG.ValidSel = cur_state->REG.sel; \
	pCtx->REG.fFlags   = CPUMSELREG_FLAGS_VALID; \
	pCtx->REG.u32Limit = cur_state->REG.limit; \
	pCtx->REG.u64Base  = cur_state->REG.base; \
	pCtx->REG.Attr.u   = cur_state->REG.access

#define GENODE_WRITE_SELREG(REG) \
	Assert(pCtx->REG.fFlags & CPUMSELREG_FLAGS_VALID); \
	Assert(pCtx->REG.ValidSel == pCtx->REG.Sel); \
	cur_state->REG.sel   = pCtx->REG.Sel; \
	cur_state->REG.limit = pCtx->REG.u32Limit; \
	cur_state->REG.base  = pCtx->REG.u64Base; \
	\
	/* attribute fixup according to 'VMX_WRITE_SELREG' in 'HWVMXR0.h' */ \
	if (( pCtx->REG.Sel \
	   || !CPUMIsGuestInPagedProtectedModeEx(pCtx) \
	   || (!pCtx->cs.Attr.n.u1DefBig && !CPUMIsGuestIn64BitCodeEx(pCtx))) \
	  && pCtx->REG.Attr.n.u1Present == 1) \
	{ \
		cur_state->REG.access = pCtx->REG.Attr.u | X86_SEL_TYPE_ACCESSED; \
	} else { \
		cur_state->REG.access = VMCS_SEG_UNUSABLE; \
	}

static Genode::Vm_handler vm_handler;

/* VirtualBox SUPLib interface */

inline bool continue_hw_accelerated(PVMR0 pVMR0, VMCPUID idCpu)
{
	VM     * pVM   = reinterpret_cast<VM *>(pVMR0);
	PVMCPU   current_vcpu = &pVM->aCpus[idCpu];

	uint32_t check_vm = VM_FF_HM_TO_R3_MASK | VM_FF_REQUEST
	                    | VM_FF_PGM_POOL_FLUSH_PENDING
	                    | VM_FF_PDM_DMA;
	uint32_t check_vcpu = VMCPU_FF_HM_TO_R3_MASK
	                      | VMCPU_FF_PGM_SYNC_CR3
	                      | VMCPU_FF_PGM_SYNC_CR3_NON_GLOBAL
	                      | VMCPU_FF_REQUEST;

	if (!VM_FF_IS_PENDING(pVM, check_vm) &&
	    !VMCPU_FF_IS_PENDING(current_vcpu, check_vcpu))
		return true;

	Assert(!(VM_FF_IS_PENDING(pVM, VM_FF_PGM_NO_MEMORY)));

	return false;
}


inline bool has_pending_irq(PVMCPU pVCpu)
{
	if (!TRPMHasTrap(pVCpu) &&
		!VMCPU_FF_IS_PENDING(pVCpu, (VMCPU_FF_INTERRUPT_APIC |
		                             VMCPU_FF_INTERRUPT_PIC)))
		return false;

	return true;
}


inline void inject_irq(PVMCPU pVCpu)
{
	int rc;

	if (!TRPMHasTrap(pVCpu)) {
		bool res = VMCPU_FF_TEST_AND_CLEAR(pVCpu, VMCPU_FF_INTERRUPT_NMI);
		Assert(!res);

		if (VMCPU_FF_IS_PENDING(pVCpu, (VMCPU_FF_INTERRUPT_APIC |
						VMCPU_FF_INTERRUPT_PIC))) {

			uint8_t irq;
			rc = PDMGetInterrupt(pVCpu, &irq);
			Assert(RT_SUCCESS(rc));

			rc = TRPMAssertTrap(pVCpu, irq, TRPM_HARDWARE_INT);
			Assert(RT_SUCCESS(rc));
		}
	}

	Assert(TRPMHasTrap(pVCpu));

	uint8_t   u8Vector;
	TRPMEVENT enmType;
	RTGCUINT  u32ErrorCode;

	/* If a new event is pending, then dispatch it now. */
	rc = TRPMQueryTrapAll(pVCpu, &u8Vector, &enmType, 0, 0, 0);
	AssertRC(rc);
	Assert(enmType == TRPM_HARDWARE_INT);
	Assert(u8Vector != X86_XCPT_NMI);

	/* Clear the pending trap. */
	rc = TRPMResetTrap(pVCpu);
	AssertRC(rc);

	switch (u8Vector) {
		case 48: // Timer
			asm volatile ("vmcall" : : "a" (2) : "memory");
			break;
		case 49: // Kbd
			asm volatile ("vmcall" : : "a" (3) : "memory");
			break;
		case 60: // Ptr
			asm volatile ("vmcall" : : "a" (6) : "memory");
			break;
		case 63: // Ata
			asm volatile ("vmcall" : : "a" (4) : "memory");
			break;
		case 239: // LOC (Local timer interrupts)
			asm volatile ("vmcall" : : "a" (5) : "memory");
			break;
		case 246: // Work
			asm volatile ("vmcall" : : "a" (7) : "memory");
			break;
		default:
			PDBG("No event to inject interrupt %u", u8Vector);
	}
}


int SUPR3QueryVTxSupported(void) { return VINF_SUCCESS; }

int SUPR3CallVMMR0Fast(PVMR0 pVMR0, unsigned uOperation, VMCPUID idCpu)
{
	static Genode::Attached_io_mem_dataspace subject_state(0xf00000000, 0x1000);

	switch (uOperation) {
	case SUP_VMMR0_DO_HM_RUN:
		struct Subject_state *cur_state = subject_state.local_addr<struct Subject_state>();
		VM     * pVM   = reinterpret_cast<VM *>(pVMR0);
		PVMCPU   pVCpu = &pVM->aCpus[idCpu];
		PCPUMCTX pCtx  = CPUMQueryGuestCtxPtr(pVCpu);

		if (has_pending_irq(pVCpu))
			inject_irq(pVCpu);

		cur_state->Rip = pCtx->rip;
		cur_state->Rsp = pCtx->rsp;

		cur_state->Regs.Rax = pCtx->rax;
		cur_state->Regs.Rbx = pCtx->rbx;
		cur_state->Regs.Rcx = pCtx->rcx;
		cur_state->Regs.Rdx = pCtx->rdx;
		cur_state->Regs.Rbp = pCtx->rbp;
		cur_state->Regs.Rsi = pCtx->rsi;
		cur_state->Regs.Rdi = pCtx->rdi;
		cur_state->Regs.R08 = pCtx->r8;
		cur_state->Regs.R09 = pCtx->r9;
		cur_state->Regs.R10 = pCtx->r10;
		cur_state->Regs.R11 = pCtx->r11;
		cur_state->Regs.R12 = pCtx->r12;
		cur_state->Regs.R13 = pCtx->r13;
		cur_state->Regs.R14 = pCtx->r14;
		cur_state->Regs.R15 = pCtx->r15;

		cur_state->Rflags = pCtx->rflags.u;

		cur_state->Sysenter_cs  = pCtx->SysEnter.cs;
		cur_state->Sysenter_eip = pCtx->SysEnter.eip;
		cur_state->Sysenter_esp = pCtx->SysEnter.esp;

		cur_state->Shadow_cr0 = pCtx->cr0;
		cur_state->Cr0  = pCtx->cr0;
		cur_state->Cr0 |= 1 << 5;
		cur_state->Regs.Cr2 = pCtx->cr2;
		cur_state->Cr3  = pCtx->cr3;
		cur_state->Cr4  = pCtx->cr4;
		cur_state->Cr4  |= 1 << 13;

		GENODE_WRITE_SELREG(cs);
		GENODE_WRITE_SELREG(ss);
		GENODE_WRITE_SELREG(ds);
		GENODE_WRITE_SELREG(es);
		GENODE_WRITE_SELREG(fs);
		GENODE_WRITE_SELREG(gs);

		if (pCtx->ldtr.Sel == 0) {
			cur_state->ldtr.sel    = 0;
			cur_state->ldtr.limit  = 0;
			cur_state->ldtr.base   = 0;
			cur_state->ldtr.access = 0x82;
		} else {
			cur_state->ldtr.sel    = pCtx->ldtr.Sel;
			cur_state->ldtr.limit  = pCtx->ldtr.u32Limit;
			cur_state->ldtr.base   = pCtx->ldtr.u64Base;
			cur_state->ldtr.access = pCtx->ldtr.Attr.u;
		}
		Assert(pCtx->tr.Attr.u & X86_SEL_TYPE_SYS_TSS_BUSY_MASK);
		{
			cur_state->tr.sel    = pCtx->tr.Sel;
			cur_state->tr.limit  = pCtx->tr.u32Limit;
			cur_state->tr.base   = pCtx->tr.u64Base;
			cur_state->tr.access = pCtx->tr.Attr.u;
		}

		cur_state->idtr.limit = pCtx->idtr.cbIdt;
		cur_state->idtr.base  = pCtx->idtr.pIdt;
		cur_state->gdtr.limit = pCtx->gdtr.cbGdt;
		cur_state->gdtr.base  = pCtx->gdtr.pGdt;

		cur_state->Ia32_efer = CPUMGetGuestEFER(pVCpu);

		/*
		utcb->dr7  = pCtx->dr[7];
		*/

		VMCPU_SET_STATE(pVCpu, VMCPUSTATE_STARTED_EXEC);

		vm_handler.run_vm();

		CPUMSetChangedFlags(pVCpu, CPUM_CHANGED_GLOBAL_TLB_FLUSH);

		VMCPU_SET_STATE(pVCpu, VMCPUSTATE_STARTED);

		pCtx->rip = cur_state->Rip;
		pCtx->rsp = cur_state->Rsp;

		pCtx->rax = cur_state->Regs.Rax;
		pCtx->rbx = cur_state->Regs.Rbx;
		pCtx->rcx = cur_state->Regs.Rcx;
		pCtx->rdx = cur_state->Regs.Rdx;
		pCtx->rbp = cur_state->Regs.Rbp;
		pCtx->rsi = cur_state->Regs.Rsi;
		pCtx->rdi = cur_state->Regs.Rdi;
		pCtx->r8  = cur_state->Regs.R08;
		pCtx->r9  = cur_state->Regs.R09;
		pCtx->r10 = cur_state->Regs.R10;
		pCtx->r11 = cur_state->Regs.R11;
		pCtx->r12 = cur_state->Regs.R12;
		pCtx->r13 = cur_state->Regs.R13;
		pCtx->r14 = cur_state->Regs.R14;
		pCtx->r15 = cur_state->Regs.R15;

		pCtx->rflags.u = cur_state->Rflags;

		if (pCtx->SysEnter.cs != cur_state->Sysenter_cs)
			CPUMSetGuestMsr(pVCpu, MSR_IA32_SYSENTER_CS, cur_state->Sysenter_cs);
		if (pCtx->SysEnter.esp != cur_state->Sysenter_esp)
			CPUMSetGuestMsr(pVCpu, MSR_IA32_SYSENTER_ESP, cur_state->Sysenter_esp);
		if (pCtx->SysEnter.eip != cur_state->Sysenter_eip)
			CPUMSetGuestMsr(pVCpu, MSR_IA32_SYSENTER_EIP, cur_state->Sysenter_eip);

		if (pCtx->idtr.cbIdt != cur_state->idtr.limit ||
		    pCtx->idtr.pIdt  != cur_state->idtr.base)
			CPUMSetGuestIDTR(pVCpu, cur_state->idtr.base, cur_state->idtr.limit);
		if (pCtx->gdtr.cbGdt != cur_state->gdtr.limit ||
		    pCtx->gdtr.pGdt  != cur_state->gdtr.base)
			CPUMSetGuestGDTR(pVCpu, cur_state->gdtr.base, cur_state->gdtr.limit);

		{
			uint32_t val;
			val  = (cur_state->Shadow_cr0 & pVCpu->hm.s.vmx.u32CR0Mask);
			val |= (cur_state->Cr0 & ~pVCpu->hm.s.vmx.u32CR0Mask);
			if (pCtx->cr0 != val)
				CPUMSetGuestCR0(pVCpu, val);
		}
		if (pCtx->cr2 != cur_state->Regs.Cr2)
			CPUMSetGuestCR2(pVCpu, cur_state->Regs.Cr2);
		if (pCtx->cr4 != cur_state->Cr4)
			CPUMSetGuestCR4(pVCpu, cur_state->Cr4);

		/*
		 * Guest CR3 must be handled after saving CR0 & CR4.
		 * See HMVMXR0.cpp, function hmR0VmxSaveGuestControlRegs
		 */
		if (pCtx->cr3 != cur_state->Cr3) {
			CPUMSetGuestCR3(pVCpu, cur_state->Cr3);
		}

		GENODE_READ_SELREG(cs);
		GENODE_READ_SELREG(ss);
		GENODE_READ_SELREG(ds);
		GENODE_READ_SELREG(es);
		GENODE_READ_SELREG(fs);
		GENODE_READ_SELREG(gs);

		if (GENODE_READ_SELREG_REQUIRED(ldtr)) {
			GENODE_READ_SELREG(ldtr);
			CPUMSetChangedFlags(pVCpu, CPUM_CHANGED_LDTR);
		}
		if (GENODE_READ_SELREG_REQUIRED(tr)) {
			GENODE_READ_SELREG(tr);
			CPUMSetChangedFlags(pVCpu, CPUM_CHANGED_TR);
		}

		CPUMSetGuestEFER(pVCpu, cur_state->Ia32_efer);

		VMCPU_FF_CLEAR(pVCpu, VMCPU_FF_TO_R3);

		if (cur_state->Intr_state != 0) {
			Assert(cur_state->Intr_state == BLOCKING_BY_STI ||
			       cur_state->Intr_state == BLOCKING_BY_MOV_SS);
			EMSetInhibitInterruptsPC(pVCpu, pCtx->rip);
		} else
			VMCPU_FF_CLEAR(pVCpu, VMCPU_FF_INHIBIT_INTERRUPTS);

#ifdef VBOX_WITH_REM
		/* XXX see VMM/VMMR0/HMVMXR0.cpp - not necessary every time ! XXX */
		REMFlushTBs(pVM);
#endif

		return continue_hw_accelerated(pVMR0, idCpu) ? VINF_SUCCESS : VINF_EM_RAW_EMULATE_INSTR;
	}

	return VERR_INTERNAL_ERROR;
}


static Genode::Semaphore *r0_halt_sem()
{
	static Genode::Semaphore sem;
	return &sem;
}


int SUPR3CallVMMR0Ex(PVMR0 pVMR0, VMCPUID idCpu, unsigned
                     uOperation, uint64_t u64Arg, PSUPVMMR0REQHDR pReqHdr)
{
	switch(uOperation)
	{
		case VMMR0_DO_GVMM_CREATE_VM:
			genode_VMMR0_DO_GVMM_CREATE_VM(pReqHdr);
			return VINF_SUCCESS;

		case VMMR0_DO_GVMM_SCHED_HALT:
			r0_halt_sem()->down();
			return VINF_SUCCESS;

		case VMMR0_DO_GVMM_SCHED_WAKE_UP:
			r0_halt_sem()->up();
			return VINF_SUCCESS;

		case VMMR0_DO_VMMR0_INIT:
			{
				VM     * pVM   = reinterpret_cast<VM *>(pVMR0);
				PVMCPU   pVCpu = &pVM->aCpus[idCpu];
				pVM->hm.s.svm.fSupported = false;
				pVM->hm.s.vmx.fSupported = true;
				pVM->hm.s.vmx.fAllowUnrestricted = false;
				pVCpu->hm.s.vmx.u32CR0Mask = 0x60000020;
				pVCpu->hm.s.vmx.u32CR4Mask = 0x2000;
				return VINF_SUCCESS;
			}
		case VMMR0_DO_GVMM_SCHED_POLL:
		case VMMR0_DO_GVMM_DESTROY_VM:
		case VMMR0_DO_VMMR0_TERM:
		case VMMR0_DO_HM_SETUP_VM:
		case VMMR0_DO_HM_ENABLE:
			return VINF_SUCCESS;

		case VMMR0_DO_GVMM_SCHED_POKE:
			vm_handler.poke();
			return VINF_SUCCESS;

		default:
			PERR("SUPR3CallVMMR0Ex: unhandled uOperation %d", uOperation);
			return VERR_GENERAL_FAILURE;
	}
}


bool create_emt_vcpu(pthread_t * thread, size_t stack_size,
                     const pthread_attr_t *attr,
                     void *(*start_routine)(void *), void *arg,
                     Genode::Cpu_session * cpu_session,
                     Genode::Affinity::Location location)
{
	/* no hardware acceleration support */
	return false;
}


/**
 * Dummies and unimplemented stuff.
 */

uint64_t genode_cpu_hz() {
	return 1000000000ULL; /* XXX fixed 1GHz return value */
}


bool Vmm_memory::revoke_from_vm(Mem_region *r)
{
	PWRN("%s unimplemented", __func__);
	return false;
}


extern "C" void pthread_yield() { PWRN("%s unimplemented", __func__); }
