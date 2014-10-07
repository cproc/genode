/*
 * \brief  VirtualBox page manager (PGM)
 * \author Norman Feske
 * \date   2013-08-20
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <util/string.h>

#include <vmm/printf.h>

/* VirtualBox includes */
#include "PGMInternal.h" /* enable access to pgm.s.* */
#include <VBox/vmm/mm.h>
#include <VBox/vmm/vm.h>
#include <VBox/vmm/pgm.h>
#include <VBox/vmm/rem.h>
#include <iprt/err.h>

/* local includes */
#include "util.h"
#include "vmm_memory.h"
#include "guest_memory.h"

using Genode::Ram_session;
using Genode::Rm_session;

static bool debug = true;

Vmm_memory *vmm_memory()
{
	static Vmm_memory inst(*Genode::env()->ram_session());
	return &inst;
}


Guest_memory *guest_memory()
{
	static Guest_memory inst;
	return &inst;
}


int PGMR3PhysRomRegister(PVM pVM, PPDMDEVINS pDevIns, RTGCPHYS GCPhys,
                         RTGCPHYS cb, const void *pvBinary, uint32_t cbBinary,
                         uint32_t fFlags, const char *pszDesc)
{
	PLOG("PGMR3PhysRomRegister: GCPhys=0x%llx cb=0x%llx pvBinary=0x%p",
	     GCPhys, cb, pvBinary);

	try {
		guest_memory()->add_rom_mapping(GCPhys, cb, pvBinary, pDevIns);

		/*
		 * XXX Try to understand the fShadowed condition
		 *     (see pgmR3PhysRomRegister)
		 */
		REMR3NotifyPhysRomRegister(pVM, GCPhys, cb, NULL, false /* fShadowed */);

	} catch (Guest_memory::Region_conflict) {
		return VERR_PGM_MAPPING_CONFLICT; }

	return VINF_SUCCESS;
}


int PGMPhysWrite(PVM pVM, RTGCPHYS GCPhys, const void *pvBuf, size_t cbWrite)
{
	void *pv = guest_memory()->lookup(GCPhys, cbWrite);

	if (pv) {
		memcpy(pv, pvBuf, cbWrite);
		return VINF_SUCCESS;
	}

	PFNPGMR3PHYSHANDLER  pfnHandlerR3 = 0;
	void                *pvUserR3     = 0;

	pv = vmm_memory()->lookup(GCPhys, cbWrite, &pfnHandlerR3, &pvUserR3);

	if (!pv || !pfnHandlerR3 || !pvUserR3) {
		PERR("PGMPhysWrite skipped: GCPhys=0x%lx pvBuf=0x%p cbWrite=0x%zx",
		     GCPhys, pvBuf, cbWrite);
		return VERR_GENERAL_FAILURE;
	}

	int rc = pfnHandlerR3(pVM, GCPhys, 0, 0, cbWrite, PGMACCESSTYPE_WRITE,
	                      pvUserR3);

	if (rc != VINF_PGM_HANDLER_DO_DEFAULT) {
		PERR("unexpected %s return code %d", __FUNCTION__, rc);
		return VERR_GENERAL_FAILURE;
	}

	memcpy(pv, pvBuf, cbWrite);
	return VINF_SUCCESS;
}


int PGMR3PhysWriteExternal(PVM pVM, RTGCPHYS GCPhys, const void *pvBuf,
                           size_t cbWrite, const char *pszWho)
{
//	PDBG("GCPhys=0x%llx pvBuf=0x%p cbWrite=0x%zx - '%s'",
//	     GCPhys, pvBuf, cbWrite, pszWho);

	return PGMPhysWrite(pVM, GCPhys, pvBuf, cbWrite);
}


int PGMPhysRead(PVM pVM, RTGCPHYS GCPhys, void *pvBuf, size_t cbRead)
{
	void *pv = guest_memory()->lookup(GCPhys, cbRead);

//	PLOG("PGMPhysRead: GCPhys=0x%x pvBuf=0x%p cbRead=0x%zx pv=%p",
//	     GCPhys, pvBuf, cbRead, pv);

	if (pv) {
		memcpy(pvBuf, pv, cbRead);
		return VINF_SUCCESS;
	}

	PFNPGMR3PHYSHANDLER  pfnHandlerR3 = 0;
	void                *pvUserR3     = 0;

	pv = vmm_memory()->lookup(GCPhys, cbRead, &pfnHandlerR3, &pvUserR3);
	if (!pv || !pfnHandlerR3 || !pvUserR3) {
		PERR("PGMPhysRead skipped: GCPhys=0x%lx pvBuf=0x%p cbRead=0x%zx",
		     GCPhys, pvBuf, cbRead);
		return VERR_GENERAL_FAILURE;
	}

	memcpy(pvBuf, pv, cbRead);
	return VINF_SUCCESS;
}


int PGMR3PhysReadExternal(PVM pVM, RTGCPHYS GCPhys, void *pvBuf, size_t cbRead)
{
	void *pv = guest_memory()->lookup(GCPhys, cbRead);

//	PLOG("PGMPhysReadExternal: GCPhys=0x%x pvBuf=0x%p cbRead=0x%zx pv=%p",
//	     GCPhys, pvBuf, cbRead, pv);

	return PGMPhysRead(pVM, GCPhys, pvBuf, cbRead);
}


int PGMR3PhysMMIO2Register(PVM pVM, PPDMDEVINS pDevIns, uint32_t iRegion,
                           RTGCPHYS cb, uint32_t fFlags, void **ppv, const char
                           *pszDesc)
{
	*ppv = vmm_memory()->alloc((size_t)cb, pDevIns, iRegion);

	PLOG("PGMR3PhysMMIO2Register: pszDesc=%s iRegion=%u cb=0x%zx -> 0x%p",
	     pszDesc, iRegion, (size_t)cb, *ppv);

	return VINF_SUCCESS;
}


int PGMR3PhysMMIO2Map(PVM pVM, PPDMDEVINS pDevIns, uint32_t iRegion,
                      RTGCPHYS GCPhys)
{
	size_t cb = vmm_memory()->map_to_vm(pDevIns, iRegion, GCPhys);
	if (cb == 0) {
		PERR("PGMR3PhysMMIO2Map: lookup for pDevIns=%p iRegion=%u failed\n",
		     pDevIns, iRegion);
		for (;;);
	}

	PLOG("PGMR3PhysMMIO2Map: pDevIns=%p iRegion=%u cb=0x%zx GCPhys=0x%lx\n",
	     pDevIns, iRegion, cb, (long)GCPhys);

	REMR3NotifyPhysRamRegister(pVM, GCPhys, cb, REM_NOTIFY_PHYS_RAM_FLAGS_MMIO2);

	return VINF_SUCCESS;
}


int PGMR3PhysMMIO2Unmap(PVM pVM, PPDMDEVINS pDevIns, uint32_t iRegion,
                        RTGCPHYS GCPhys)
{
	if (debug)
		PDBG("called phys=%llx iRegion=0x%x", GCPhys, iRegion);

	vmm_memory()->map_to_vm(pDevIns, iRegion, 0);

	return VINF_SUCCESS;
}


bool PGMR3PhysMMIO2IsBase(PVM pVM, PPDMDEVINS pDevIns, RTGCPHYS GCPhys)
{
	bool res = vmm_memory()->lookup(GCPhys, 1);
	if (debug)
		PDBG("called phys=%llx res=%u", GCPhys, res);
	return res;
}

	
int PGMR3HandlerPhysicalRegister(PVM pVM, PGMPHYSHANDLERTYPE enmType,
                                 RTGCPHYS GCPhys, RTGCPHYS GCPhysLast,
                                 PFNPGMR3PHYSHANDLER pfnHandlerR3,
                                 void *pvUserR3, const char *pszModR0,
                                 const char *pszHandlerR0, RTR0PTR pvUserR0,
                                 const char *pszModRC,
                                 const char *pszHandlerRC,
                                 RTRCPTR pvUserRC, const char *pszDesc)
{
	PLOG("PGMR3HandlerPhysicalRegister: pszDesc=%s %u GCPhys=0x%llx"
	     " GCPhysLast=0x%llx r3=0x%p enmType=%x\n",
	     pszDesc, enmType, GCPhys, GCPhysLast, pfnHandlerR3, enmType);

#ifdef VBOX_WITH_REM
	REMR3NotifyHandlerPhysicalRegister(pVM, enmType, GCPhys, GCPhysLast -
	                                   GCPhys + 1, !!pfnHandlerR3);
#endif

	bool ok = vmm_memory()->add_handler(GCPhys, GCPhysLast - GCPhys + 1,
	                                    pfnHandlerR3, pvUserR3, enmType);
	Assert(ok);

	return VINF_SUCCESS;
}


int PGMHandlerPhysicalDeregister(PVM pVM, RTGCPHYS GCPhys)
{
	size_t size = 1;

#ifdef VBOX_WITH_REM
	PFNPGMR3PHYSHANDLER pfnHandlerR3 = 0;
	PGMPHYSHANDLERTYPE enmType;

	void * pv = vmm_memory()->lookup(GCPhys, size, &pfnHandlerR3, 0, &enmType);
	Assert(pv);

	if (debug)
		PDBG("called phys=%llx enmType=%x", GCPhys, enmType);

#endif

	bool ok = vmm_memory()->add_handler(GCPhys, size, 0, 0);
	Assert(ok);

#ifdef VBOX_WITH_REM
    bool fRestoreAsRAM = pfnHandlerR3 && enmType != PGMPHYSHANDLERTYPE_MMIO;

	/* GCPhysstart and size gets written ! */
	RTGCPHYS GCPhysStart = GCPhys;
	bool io = vmm_memory()->lookup_range(GCPhysStart, size);
	Assert(io);

    REMR3NotifyHandlerPhysicalDeregister(pVM, enmType, GCPhysStart, size,
                                         !!pfnHandlerR3, fRestoreAsRAM);
#endif

	return VINF_SUCCESS;
}


int PGMR3PhysRegisterRam(PVM pVM, RTGCPHYS GCPhys, RTGCPHYS cb,
                         const char *pszDesc)
{
	PLOG("PGMR3PhysRegisterRam: GCPhys=0x%lx, cb=0x%zx, pszDesc=%s",
	     (long)GCPhys, (size_t)cb, pszDesc);

	try {

		/*
		 * XXX Is this function the right place for the allocation?
		 *     The lack of allocation-related VERR_PGM_ error codes suggests
		 *     so.
		 */
		void *pv = vmm_memory()->alloc_ram((size_t)cb);

		guest_memory()->add_ram_mapping(GCPhys, cb, pv);

		REMR3NotifyPhysRamRegister(pVM, GCPhys, cb, REM_NOTIFY_PHYS_RAM_FLAGS_RAM);
	}
	catch (Guest_memory::Region_conflict) {
		return VERR_PGM_MAPPING_CONFLICT; }
	catch (Ram_session::Alloc_failed) {
		return VERR_PGM_MAPPING_CONFLICT; /* XXX use a better error code? */ }
	catch (Rm_session::Attach_failed) {
		return VERR_PGM_MAPPING_CONFLICT; /* XXX use a better error code? */ }

	return VINF_SUCCESS;
}


int PGMMapSetPage(PVM pVM, RTGCPTR GCPtr, uint64_t cb, uint64_t fFlags)
{
	PLOG("PGMMapSetPage: GCPtr=0x%llx cb=0x%llx, flags=0x%llx",
	     GCPtr, cb, fFlags);

	return VINF_SUCCESS;
}


RTHCPHYS PGMGetHyperCR3(PVMCPU pVCpu)
{
//	PDBG("%s %lx", __func__, CPUMGetHyperCR3(pVCpu));
	return 1;
}


int PGMR3Init(PVM pVM)
{
	/*
	 * Satisfy assertion in VMMR3Init. Normally called via:
	 *
	 * PGMR3Init -> pgmR3InitPaging -> pgmR3ModeDataInit -> InitData -> MapCR3
	 */
	for (VMCPUID idCpu = 0; idCpu < pVM->cCpus; idCpu++) {
		PVMCPU pVCpu = &pVM->aCpus[idCpu];
		CPUMSetHyperCR3(pVCpu, PGMGetHyperCR3(pVCpu));

		pVCpu->pgm.s.fA20Enabled      = true;
		pVCpu->pgm.s.GCPhysA20Mask    = ~((RTGCPHYS)!pVCpu->pgm.s.fA20Enabled << 20);
	}

	return VINF_SUCCESS;
}


int PGMPhysGCPtr2CCPtrReadOnly(PVMCPU pVCpu, RTGCPTR GCPtr, void const **ppv,
                           PPGMPAGEMAPLOCK pLock)
{
	PERR("%s not implemented - caller 0x%p",
	     __func__, __builtin_return_address(0));

	while (1) { asm volatile ("ud2a"); }
	return VERR_GENERAL_FAILURE;
}


int PGMR3PhysTlbGCPhys2Ptr(PVM pVM, RTGCPHYS GCPhys, bool fWritable, void **ppv)
{
	size_t const size = 1;
	void *pv = guest_memory()->lookup(GCPhys, size);

	if (pv) {
		*ppv = pv;
		return VINF_SUCCESS;
	}

	PFNPGMR3PHYSHANDLER  pfnHandlerR3 = 0;
	void                *pvUserR3     = 0;

	pv = vmm_memory()->lookup(GCPhys, size, &pfnHandlerR3, &pvUserR3);

	if (!pv) {
		PERR("%s: lookup for GCPhys=0x%p failed", __func__, GCPhys);
		return VERR_PGM_PHYS_TLB_UNASSIGNED;
	}

	/* pv valid - check handlers next */
	if (!pfnHandlerR3 && !pvUserR3) {
		*ppv = pv;
		return VINF_SUCCESS;
	}
 
	PERR("%s: denied access - handlers set - GCPhys=0x%p", __func__, GCPhys);
	return VERR_PGM_PHYS_TLB_CATCH_ALL;
}


void PGMR3PhysSetA20(PVMCPU pVCpu, bool fEnable) 
{
	if (!pVCpu->pgm.s.fA20Enabled != fEnable) {
		pVCpu->pgm.s.fA20Enabled = fEnable;
		REMR3A20Set(pVCpu->pVMR3, pVCpu, fEnable);
	}

	return;
}


bool PGMPhysIsA20Enabled(PVMCPU pVCpu)
{
	return pVCpu->pgm.s.fA20Enabled; 
}


void PGMR3PhysWriteU8(PVM pVM, RTGCPHYS GCPhys, uint8_t value)
{
	void *pv = guest_memory()->lookup(GCPhys, sizeof(value));

	if (!pv) {
		PERR("%s: invalid write attempt phy=%llx", __func__, GCPhys);
		return;
	}

	*reinterpret_cast<uint8_t *>(pv) = value;
}


void PGMR3PhysWriteU16(PVM pVM, RTGCPHYS GCPhys, uint16_t value)
{
	void *pv = guest_memory()->lookup(GCPhys, sizeof(value));

	if (!pv) {
		PERR("%s: invalid write attempt phy=%llx", __func__, GCPhys);
		return;
	}

	*reinterpret_cast<uint16_t *>(pv) = value;
}


void PGMR3PhysWriteU32(PVM pVM, RTGCPHYS GCPhys, uint32_t value)
{
	void *pv = guest_memory()->lookup(GCPhys, sizeof(value));

	if (!pv) {
		PERR("%s: invalid write attempt phy=%llx", __func__, GCPhys);
		return;
	}

	*reinterpret_cast<uint32_t *>(pv) = value;
}


uint32_t PGMR3PhysReadU32(PVM pVM, RTGCPHYS GCPhys)
{
	void *pv = guest_memory()->lookup(GCPhys, 4);

	if (!pv) {
		PERR("%s: invalid read attempt phys=%llx", __func__, GCPhys);
		return 0;
	}

	return *reinterpret_cast<uint32_t *>(pv);
}


int PGMPhysGCPhys2CCPtrReadOnly(PVM pVM, RTGCPHYS GCPhys, void const **ppv,
                                PPGMPAGEMAPLOCK pLock)
{
	void *pv = guest_memory()->lookup(GCPhys, 0x1000);

	if (!pv) {
		PERR("unknown address pv=%p ppv=%p GCPhys=%llx", pv, ppv, GCPhys);

		guest_memory()->dump();

		return VERR_PGM_INVALID_GC_PHYSICAL_ADDRESS;
	}

	*ppv = pv;

	PVMCPU pVCpu = VMMGetCpu(pVM);

	return VINF_SUCCESS;
}


int PGMHandlerPhysicalReset(PVM, RTGCPHYS GCPhys)
{
	if (!vmm_memory()->unmap_from_vm(GCPhys))
		PWRN("%s: unbacked region - GCPhys %lx", __func__, GCPhys);

	return VINF_SUCCESS;
}


extern "C" int MMIO2_MAPPED_SYNC(PVM pVM, RTGCPHYS GCPhys, size_t cbWrite,
                                 void **ppv)
{
	/* DON'T USE normal printf in this function - corrupts unsaved UTCB !!! */

	PFNPGMR3PHYSHANDLER  pfnHandlerR3 = 0;
	void                *pvUserR3     = 0;

	void * pv = vmm_memory()->lookup(GCPhys, cbWrite, &pfnHandlerR3, &pvUserR3);

	if (!pv)
		return VERR_PGM_PHYS_TLB_UNASSIGNED;

	if (!pfnHandlerR3 && !pvUserR3) {
		*ppv = pv;
		/* you may map it */
		return VINF_SUCCESS;
	}

	if (pfnHandlerR3 && pvUserR3) {
		int rc = pfnHandlerR3(pVM, GCPhys, 0, 0, cbWrite, PGMACCESSTYPE_WRITE,
		                      pvUserR3);

		if (rc == VINF_PGM_HANDLER_DO_DEFAULT) {
			*ppv = pv;
			/* you may map it */
			return VINF_SUCCESS;
		}
		
		Vmm::printf("%s: GCPhys=0x%lx failed - unexpected rc=%d\n",
		            __func__, GCPhys, rc);
		return rc;
	}

	Vmm::printf("%s: GCPhys=0x%lx failed - unexpected state \n",
	            __func__, GCPhys);
	return VERR_GENERAL_FAILURE;
}


void PGMR3Reset(PVM pVM)
{
	VM_ASSERT_EMT(pVM);

	for (VMCPUID i = 0; i < pVM->cCpus; i++)
	{
//		int rc = PGMR3ChangeMode(pVM, &pVM->aCpus[i], PGMMODE_REAL);
//		AssertRC(rc);
	}

	for (VMCPUID i = 0; i < pVM->cCpus; i++)
	{
		PVMCPU pVCpu = &pVM->aCpus[i];

		VMCPU_FF_CLEAR(pVCpu, VMCPU_FF_PGM_SYNC_CR3);
		VMCPU_FF_CLEAR(pVCpu, VMCPU_FF_PGM_SYNC_CR3_NON_GLOBAL);

		if (!pVCpu->pgm.s.fA20Enabled)
		{
			pVCpu->pgm.s.fA20Enabled = true;
			pVCpu->pgm.s.GCPhysA20Mask = ~((RTGCPHYS)!pVCpu->pgm.s.fA20Enabled << 20);
#ifdef PGM_WITH_A20
			pVCpu->pgm.s.fSyncFlags |= PGM_SYNC_UPDATE_PAGE_BIT_VIRTUAL;
			VMCPU_FF_SET(pVCpu, VMCPU_FF_PGM_SYNC_CR3);
			HWACCMFlushTLB(pVCpu);
#endif
		}
	}

	PERR("clearing ram and rom areas missing !!!!!!!");	
}


int PGMR3MappingsSize(PVM pVM, uint32_t *pcb)
{
	PINF("%s - not implemented - %p", __func__, __builtin_return_address(0));

	*pcb = 0;

	return 0;
}
