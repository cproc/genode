/*
 * \brief  VirtualBox SUPLib supplements
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
#include <os/attached_ram_dataspace.h>
#include <base/semaphore.h>
#include <os/timed_semaphore.h>
#include <trace/timestamp.h>

/* Genode/Virtualbox includes */
#include "sup.h"

/* VirtualBox includes */
#include <iprt/semaphore.h>
#include <iprt/ldr.h>
#include <iprt/uint128.h>
#include <VBox/err.h>


struct Attached_gip : Genode::Attached_ram_dataspace
{
	Attached_gip()
	: Attached_ram_dataspace(Genode::env()->ram_session(), PAGE_SIZE)
	{ }
};


enum {
	UPDATE_HZ  = 250,                     /* Hz */
	/* Note: UPDATE_MS < 10ms is not supported by alarm timer, take care !*/
	UPDATE_MS  = 1000 / UPDATE_HZ,
	UPDATE_NS  = UPDATE_MS * 1000 * 1000,
};


PSUPGLOBALINFOPAGE g_pSUPGlobalInfoPage;


class Periodic_GIP : public Genode::Alarm {

	bool on_alarm(unsigned) override
	{
		/**
		 * We're using rdtsc here since timer_session->elapsed_ms produces
		 * instable results when the timer service is using the Genode PIC
		 * driver as done for base-nova currently.
		 */

		static uint64_t tsc_reference = Genode::Trace::timestamp();

		Genode::uint64_t tsc_current = Genode::Trace::timestamp() - tsc_reference;

		/*
		 * Convert tsc to nanoseconds.
		 *
		 * There is no 'uint128_t' type on x86_32, so we use the 128-bit type
		 * and functions provided by VirtualBox.
		 *
		 * nanots128 = tsc_current * 1000*1000*1000 / genode_cpu_hz()
		 *
		 */

		RTUINT128U nanots128;
		RTUInt128AssignU64(&nanots128, tsc_current);

		RTUINT128U multiplier;
		RTUInt128AssignU32(&multiplier, 1000*1000*1000);
		RTUInt128AssignMul(&nanots128, &multiplier);

		RTUINT128U divisor;
		RTUInt128AssignU64(&divisor, genode_cpu_hz());
		RTUInt128AssignDiv(&nanots128, &divisor);

		SUPGIPCPU *cpu = &g_pSUPGlobalInfoPage->aCPUs[0];

		/*
		 * Transaction id must be incremented before and after update,
		 * read struct SUPGIPCPU description for more details.
		 */
		ASMAtomicIncU32(&cpu->u32TransactionId);

		cpu->u64TSC    = tsc_current;
		cpu->u64NanoTS = nanots128.s.Lo;

		/*
		 * Transaction id must be incremented before and after update,
		 * read struct SUPGIPCPU description for more details.
		 */
		ASMAtomicIncU32(&cpu->u32TransactionId);

		return true;
	}
};



int SUPR3Init(PSUPDRVSESSION *ppSession)
{
	static bool initialized = false;

	if (initialized) return VINF_SUCCESS;

	static Attached_gip gip;

	g_pSUPGlobalInfoPage = gip.local_addr<SUPGLOBALINFOPAGE>();

	/* checked by TMR3Init */
	g_pSUPGlobalInfoPage->u32Version            = SUPGLOBALINFOPAGE_VERSION;
	g_pSUPGlobalInfoPage->u32Magic              = SUPGLOBALINFOPAGE_MAGIC;
	g_pSUPGlobalInfoPage->u32Mode               = SUPGIPMODE_SYNC_TSC;
	g_pSUPGlobalInfoPage->cCpus                 = 1;
	g_pSUPGlobalInfoPage->cPages                = 1;
	g_pSUPGlobalInfoPage->u32UpdateHz           = UPDATE_HZ;
	g_pSUPGlobalInfoPage->u32UpdateIntervalNS   = UPDATE_NS;
//	g_pSUPGlobalInfoPage->u64NanoTSLastUpdateHz =
//	g_pSUPGlobalInfoPage->OnlineCpuSet          =
//	g_pSUPGlobalInfoPage->PresentCpuSet         =
//	g_pSUPGlobalInfoPage->PossibleCpuSet        =
	g_pSUPGlobalInfoPage->cOnlineCpus           = 0;
	g_pSUPGlobalInfoPage->cPresentCpus          = 0;
	g_pSUPGlobalInfoPage->cPossibleCpus         = 0;
	g_pSUPGlobalInfoPage->idCpuMax              = 0;

	SUPGIPCPU *cpu = &g_pSUPGlobalInfoPage->aCPUs[0];

	cpu->u32TransactionId        = 0;
	cpu->u32UpdateIntervalTSC    = genode_cpu_hz() / UPDATE_HZ;
	cpu->u64NanoTS               = 0ULL;
	cpu->u64TSC                  = 0ULL;
	cpu->u64CpuHz                = genode_cpu_hz();
	cpu->cErrors                 = 0;
	cpu->iTSCHistoryHead         = 0;
//	cpu->au32TSCHistory[8]       =
	cpu->u32PrevUpdateIntervalNS = UPDATE_NS;
	cpu->enmState                = SUPGIPCPUSTATE_ONLINE;
	cpu->idCpu                   = 0;
	cpu->iCpuSet                 = 0;
	cpu->idApic                  = 0;

	/* schedule periodic call of GIP update function */
	static Periodic_GIP alarm;
	Genode::Timeout_thread::alarm_timer()->schedule(&alarm, UPDATE_MS);

	initialized = true;

	return VINF_SUCCESS;
}


int SUPR3Term(bool) { return VINF_SUCCESS; }


int SUPR3GipGetPhys(PRTHCPHYS pHCPhys)
{
	/*
	 * Return VMM-local address as physical address. This address is
	 * then fed to MMR3HyperMapHCPhys. (TMR3Init)
	 */
	*pHCPhys = (RTHCPHYS)g_pSUPGlobalInfoPage;

	return VINF_SUCCESS;
}


int SUPR3HardenedLdrLoadAppPriv(const char *pszFilename, PRTLDRMOD phLdrMod,
                               uint32_t fFlags, PRTERRINFO pErrInfo)
{
	return RTLdrLoad(pszFilename, phLdrMod);
}


uint32_t SUPSemEventMultiGetResolution(PSUPDRVSESSION)
{
	return 100000*10; /* called by 'vmR3HaltGlobal1Init' */
}

int SUPSemEventCreate(PSUPDRVSESSION pSession, PSUPSEMEVENT phEvent)
{
	*phEvent = (SUPSEMEVENT)new Genode::Semaphore();
	return VINF_SUCCESS;
}


int SUPSemEventClose(PSUPDRVSESSION pSession, SUPSEMEVENT hEvent)
{
	if (hEvent)
		delete reinterpret_cast<Genode::Semaphore *>(hEvent);
	return VINF_SUCCESS;
}


int SUPSemEventSignal(PSUPDRVSESSION pSession, SUPSEMEVENT hEvent)
{
	if (hEvent)
		reinterpret_cast<Genode::Semaphore *>(hEvent)->up();
	else
		PERR("%s called - not implemented", __FUNCTION__);

	return VINF_SUCCESS;	
}


int SUPSemEventWaitNoResume(PSUPDRVSESSION pSession, SUPSEMEVENT hEvent,
                            uint32_t cMillies)
{
	if (hEvent && cMillies == RT_INDEFINITE_WAIT)
		reinterpret_cast<Genode::Semaphore *>(hEvent)->down();
	else {
		PERR("%s called millis=%u - not implemented", __FUNCTION__, cMillies);
		reinterpret_cast<Genode::Semaphore *>(hEvent)->down();
	}

	return VINF_SUCCESS;	
}


SUPDECL(int) SUPSemEventMultiCreate(PSUPDRVSESSION,
                                    PSUPSEMEVENTMULTI phEventMulti)
{
    RTSEMEVENTMULTI sem;

    /*
     * Input validation.
     */
    AssertPtrReturn(phEventMulti, VERR_INVALID_POINTER);

    /*
     * Create the event semaphore object.
     */
	int rc = RTSemEventMultiCreate(&sem);

	static_assert(sizeof(sem) == sizeof(*phEventMulti), "oi");
	*phEventMulti = reinterpret_cast<SUPSEMEVENTMULTI>(sem);
	return rc;
}


SUPDECL(int) SUPSemEventMultiClose(PSUPDRVSESSION, SUPSEMEVENTMULTI hEvMulti)
{
	return RTSemEventMultiDestroy(reinterpret_cast<RTSEMEVENTMULTI>(hEvMulti));
}


int SUPR3CallVMMR0(PVMR0 pVMR0, VMCPUID idCpu, unsigned uOperation,
                   void *pvArg)
{
	if (uOperation == VMMR0_DO_CALL_HYPERVISOR) {
		PDBG("VMMR0_DO_CALL_HYPERVISOR - doing nothing");
		return VINF_SUCCESS;
	}
	if (uOperation == VMMR0_DO_VMMR0_TERM) {
		PDBG("VMMR0_DO_VMMR0_TERM - doing nothing");
		return VINF_SUCCESS;
	}
	if (uOperation == VMMR0_DO_GVMM_DESTROY_VM) {
		PDBG("VMMR0_DO_GVMM_DESTROY_VM - doing nothing");
		return VINF_SUCCESS;
	}

	AssertMsg(uOperation != VMMR0_DO_VMMR0_TERM &&
	          uOperation != VMMR0_DO_CALL_HYPERVISOR &&
	          uOperation != VMMR0_DO_GVMM_DESTROY_VM,
	          ("SUPR3CallVMMR0Ex: unhandled uOperation %d", uOperation));
	return VERR_GENERAL_FAILURE;
}
