/*
 * \brief  Dummy implementations of symbols needed by VirtualBox
 * \author Norman Feske
 * \date   2013-08-22
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <base/printf.h>
#include <base/thread.h>

#include <stddef.h>
#include <time.h>

#include <VBox/vmm/iom.h>
#include <VBox/vmm/selm.h>
#include <VBox/vmm/hwaccm.h>

extern "C" {

	typedef long DUMMY;

#define DUMMY(retval, name) \
DUMMY name(void) { \
	PDBG( #name " called, not implemented, eip=%p", __builtin_return_address(0)); \
	for (;;); \
	return retval; \
}

#define CHECKED_DUMMY(retval, name) \
DUMMY name(void) { \
	PINF( #name " called, not implemented, eip=%p", __builtin_return_address(0)); \
	return retval; \
}

#define NOT_IMPLEMENTED_ARG(retval) \
{ \
	PDBG(" %s called, not implemented, eip=%p", __func__, \
	     __builtin_return_address(0)); \
	Genode::Lock deadlock; \
	for (;;) deadlock.lock(); \
	return retval; \
}

#define NOT_IMPLEMENTED NOT_IMPLEMENTED_ARG(0)
#define NOT_IMPLEMENTED_VOID NOT_IMPLEMENTED_ARG()

#define NOT_IMPLEMENTED_CHECKED(retval) \
{ return retval; }

CHECKED_DUMMY( 0, cpumR3DbgInit)
CHECKED_DUMMY( 0, DBGFR3Init)  /* debugger */
DUMMY(-1, DBGFR3CoreWrite)
CHECKED_DUMMY( 0, FTMR3Init)  /* fault tolerance manager */
CHECKED_DUMMY( 0, pdmR3LdrInitU) /* module loader of pluggable device manager */
CHECKED_DUMMY( 0, PDMR3LdrLoadVMMR0U) /* pretend to have successfully loaded the r0 module */
CHECKED_DUMMY( 0, pdmR3LoadR3U)
CHECKED_DUMMY( 0, pthread_atfork)
CHECKED_DUMMY( 0, pthread_attr_setdetachstate)
CHECKED_DUMMY( 0, pthread_attr_setstacksize)
CHECKED_DUMMY( 0, RTMemProtect)
CHECKED_DUMMY( 0, sigfillset)
CHECKED_DUMMY( 0, vmmR3SwitcherInit)  /* world switcher */
CHECKED_DUMMY(-1, atexit)
CHECKED_DUMMY(-1, getpid)
CHECKED_DUMMY(-1, pdmR3FileR3)
CHECKED_DUMMY(0, setlocale)
CHECKED_DUMMY(-1, sigaddset)
CHECKED_DUMMY(-1, sigemptyset)
CHECKED_DUMMY(-1, siginterrupt)
CHECKED_DUMMY(-1, sysctl)
const RTCOMERRMSG* RTErrCOMGet(uint32_t) NOT_IMPLEMENTED
void CPUMPushHyper(PVMCPU, uint32_t) NOT_IMPLEMENTED_CHECKED() /* called by 'VMMR3InitRC', but we don't use GC */
DUMMY(-1, DBGCRegisterCommands)
DUMMY(-1, DBGFR3Event)
DUMMY(-1, DBGFR3EventAssertion)
DUMMY(-1, DBGFR3EventBreakpoint)
DUMMY(-1, DBGFR3EventSrc)
CHECKED_DUMMY( 0, DBGFR3EventSrcV)
void DBGFR3Relocate() { }
DUMMY(-1, DBGFR3Term)
DUMMY(-1, DBGFR3VMMForcedAction)

CHECKED_DUMMY(-4, DBGFR3AsSymbolByAddr) /* -4 == VERR_INVALID_HANDLE */

DUMMY(-1, _flockfile)

int FTMR3SetCheckpoint() { return -1; }
DUMMY(-1, FTMR3Term)
int FTMSetCheckpoint() { return 0; }
DUMMY(-1, _funlockfile)
DUMMY(-1, _fwalk)

int HWACCMInvalidatePage(PVMCPU, RTGCPTR) NOT_IMPLEMENTED
int HWACCMFlushTLB(PVMCPU) NOT_IMPLEMENTED
int HWACCMR3EmulateIoBlock(PVM, PCPUMCTX) NOT_IMPLEMENTED
int HWACCMR3PatchTprInstr(PVM, PVMCPU, PCPUMCTX) NOT_IMPLEMENTED
void HWACCMR3CheckError(PVM, int) NOT_IMPLEMENTED_VOID
VBOXSTRICTRC HWACCMR3RestartPendingIOInstr(PVM, PVMCPU, PCPUMCTX) NOT_IMPLEMENTED
void HWACCMR3Relocate(PVM) NOT_IMPLEMENTED_CHECKED()
void HWACCMR3Reset(PVM pVM) NOT_IMPLEMENTED_CHECKED()
int HWACCMR3Term(PVM) NOT_IMPLEMENTED
int HWACMMR3EnablePatching(PVM, RTGCPTR, unsigned int) NOT_IMPLEMENTED
int HWACMMR3DisablePatching(PVM, RTGCPTR, unsigned int) NOT_IMPLEMENTED

CHECKED_DUMMY( 0, IEMR3Init)  /* interpreted execution manager (seems to be just a skeleton) */
void IEMR3Relocate() { }
DUMMY(-1, IEMR3Term)

DUMMY(-1, MMHyperR0ToCC)
DUMMY(-1, MMHyperR0ToR3)
DUMMY(-1, MMHyperRCToCC)
DUMMY(-1, MMHyperRCToR3)
CHECKED_DUMMY(0, MMHyperGetArea)

DUMMY(-1, MMR3HeapAPrintfV)
CHECKED_DUMMY( 0, MMR3HyperInitFinalize)
CHECKED_DUMMY( 0, MMR3HyperSetGuard)
DUMMY(-1, MMR3LockCall)
DUMMY(-1, MMR3Term)
DUMMY(-1, MMR3TermUVM)
DUMMY(-1, PDMR3AsyncCompletionTemplateCreateDriver)
DUMMY(-1, PDMR3LdrGetInterfaceSymbols)
CHECKED_DUMMY( 0, PDMR3LdrRelocateU)
DUMMY(-1, pdmR3LdrTermU)

void PGMNotifyNxeChanged(PVMCPU, bool) NOT_IMPLEMENTED_VOID
int PGMPhysGCPtr2GCPhys(PVMCPU, RTGCPTR, PRTGCPHYS) NOT_IMPLEMENTED
int PGMPhysSimpleReadGCPhys(PVM, void*, RTGCPHYS, size_t) NOT_IMPLEMENTED
int PGMPhysSimpleReadGCPtr(PVMCPU, void*, RTGCPTR, size_t) NOT_IMPLEMENTED
int PGMPhysSimpleWriteGCPtr(PVMCPU, RTGCPTR, const void*, size_t) NOT_IMPLEMENTED
int PGMSyncCR3(PVMCPU, uint64_t, uint64_t, uint64_t, bool) NOT_IMPLEMENTED

int PGMR3CheckIntegrity(PVM) NOT_IMPLEMENTED_CHECKED(0)
int PGMR3FinalizeMappings(PVM) NOT_IMPLEMENTED_CHECKED(0)
int PGMR3InitCompleted(PVM, VMINITCOMPLETED) NOT_IMPLEMENTED_CHECKED(0)
int PGMR3InitDynMap(PVM) NOT_IMPLEMENTED_CHECKED(0)
int PGMR3InitFinalize(PVM) NOT_IMPLEMENTED_CHECKED(0)

int PGMR3SharedModuleCheckAll(PVM) NOT_IMPLEMENTED
int PGMR3SharedModuleUnregister(PVM, char*, char*, RTGCPTR, uint32_t)
	NOT_IMPLEMENTED
int PGMR3SharedModuleRegister(PVM, VBOXOSFAMILY, char*, char*, RTGCPTR,
                              uint32_t, uint32_t,
                              const VMMDEVSHAREDREGIONDESC*) NOT_IMPLEMENTED
int PGMR3MappingsUnfix(PVM) NOT_IMPLEMENTED
int PGMR3PhysChangeMemBalloon(PVM, bool, unsigned int, RTGCPHYS*) NOT_IMPLEMENTED
int PGMR3MappingsFix(PVM, RTGCPTR, uint32_t) NOT_IMPLEMENTED
int PGMR3MappingsDisable(PVM) NOT_IMPLEMENTED
int PGMR3LockCall(PVM) NOT_IMPLEMENTED
int PGMR3PhysAllocateHandyPages(PVM) NOT_IMPLEMENTED
int PGMR3PhysAllocateLargeHandyPage(PVM, RTGCPHYS) NOT_IMPLEMENTED
int PGMR3PhysChunkMap(PVM, uint32_t) NOT_IMPLEMENTED
int PGMR3PhysGCPhys2CCPtrExternal(PVM, RTGCPHYS, void**, PPGMPAGEMAPLOCK) NOT_IMPLEMENTED
int PGMR3PhysGCPhys2CCPtrReadOnlyExternal(PVM, RTGCPHYS, const void**,
                                          PPGMPAGEMAPLOCK) NOT_IMPLEMENTED
int PGMR3PhysMMIO2Deregister(PVM, PPDMDEVINS, uint32_t) NOT_IMPLEMENTED
int PGMR3PhysMMIO2MapKernel(PVM, PPDMDEVINS, uint32_t, RTGCPHYS, RTGCPHYS,
                            const char*, PRTR0PTR) NOT_IMPLEMENTED
uint16_t PGMR3PhysReadU16(PVM, RTGCPHYS) NOT_IMPLEMENTED
uint64_t PGMR3PhysReadU64(PVM, RTGCPHYS) NOT_IMPLEMENTED
int PGMR3PhysRomProtect(PVM, RTGCPHYS, RTGCPHYS, PGMROMPROT) NOT_IMPLEMENTED
int PGMR3PoolGrow(PVM) NOT_IMPLEMENTED
void PGMR3Relocate(PVM, RTGCINTPTR) NOT_IMPLEMENTED_CHECKED()
void PGMR3ResetCpu(PVM, PVMCPU) NOT_IMPLEMENTED_VOID
int PGMR3Term(PVM) NOT_IMPLEMENTED

int PGMPrefetchPage(PVMCPU, RTGCPTR) NOT_IMPLEMENTED
int PGMGstGetPage(PVMCPU, RTGCPTR, uint64_t*, PRTGCPHYS) NOT_IMPLEMENTED
bool PGMGstIsPagePresent(PVMCPU, RTGCPTR) NOT_IMPLEMENTED
int PGMShwMakePageReadonly(PVMCPU, RTGCPTR, uint32_t) NOT_IMPLEMENTED
int PGMShwMakePageNotPresent(PVMCPU, RTGCPTR, uint32_t) NOT_IMPLEMENTED
bool PGMPhysIsGCPhysNormal(PVM, RTGCPHYS) NOT_IMPLEMENTED
int PGMHandlerVirtualChangeInvalidateCallback(PVM, RTGCPTR, PFNPGMR3VIRTINVALIDATE) NOT_IMPLEMENTED
int PGMSetLargePageUsage(PVM, bool) NOT_IMPLEMENTED
int PGMPhysSimpleDirtyWriteGCPtr(PVMCPU, RTGCPTR, const void*, size_t) NOT_IMPLEMENTED
PGMMODE PGMGetShadowMode(PVMCPU) NOT_IMPLEMENTED_ARG(PGMMODE_INVALID)
PGMMODE PGMGetHostMode(PVM) NOT_IMPLEMENTED_ARG(PGMMODE_INVALID)

CHECKED_DUMMY(0, poll)  /* needed by 'DrvHostSerial.cpp' */
DUMMY(-1, pthread_key_delete)
DUMMY(-1, RTMemExecFree)
DUMMY(-1, RTMemPageFree)
DUMMY(-1, RTPathAppend)
DUMMY(-1, RTSemEventWaitEx)

int SELMR3Init(PVM) NOT_IMPLEMENTED_CHECKED(0)
int SELMR3InitFinalize(PVM) NOT_IMPLEMENTED_CHECKED(0)
void SELMR3Relocate(PVM) NOT_IMPLEMENTED_CHECKED()
void SELMR3Reset(PVM) NOT_IMPLEMENTED_CHECKED()
void SELMR3DisableMonitoring(PVM) NOT_IMPLEMENTED_CHECKED()
int SELMR3Term(PVM) NOT_IMPLEMENTED
int SELMR3GetSelectorInfo(PVM, PVMCPU, RTSEL, PDBGFSELINFO) NOT_IMPLEMENTED

DUMMY(-1, libc_select_notify) /* needed for libc_terminal plugin */
CHECKED_DUMMY(-1, signal)

DUMMY(-1, strcat)
DUMMY(-1, strerror)
DUMMY(-1, strpbrk)

int SUPR3SetVMForFastIOCtl(PVMR0) NOT_IMPLEMENTED_CHECKED(0)
int SUPR3HardenedLdrLoadPlugIn(const char*, PRTLDRMOD, PRTERRINFO) NOT_IMPLEMENTED
int SUPR3Term(bool) NOT_IMPLEMENTED

uint32_t SUPSemEventMultiGetResolution(PSUPDRVSESSION)
	 NOT_IMPLEMENTED_CHECKED(100000*10) /* called by 'vmR3HaltGlobal1Init' */
CHECKED_DUMMY(-1, __swsetup)

void VMMR3FatalDump(PVM, PVMCPU, int) NOT_IMPLEMENTED_VOID
void vmmR3SwitcherRelocate() { }
int VMMR3DisableSwitcher(PVM) NOT_IMPLEMENTED_CHECKED(0)
RTR0PTR VMMR3GetHostToGuestSwitcher(PVM, VMMSWITCHER) NOT_IMPLEMENTED

DUMMY(-1, pthread_kill)
DUMMY(-1, sscanf)
DUMMY(-1, RTHeapSimpleRelocate)
DUMMY(-1, RTHeapSimpleAlloc)
DUMMY(-1, RTHeapSimpleInit)
DUMMY(-1, RTHeapSimpleFree)
_AVLOU32NodeCore* RTAvloU32Remove(PAVLOU32TREE, AVLOU32KEY) NOT_IMPLEMENTED
_AVLOU32NodeCore* RTAvloU32Get(PAVLOU32TREE, AVLOU32KEY) NOT_IMPLEMENTED
_AVLOU32NodeCore* RTAvloU32GetBestFit(PAVLOU32TREE, AVLOU32KEY, bool) NOT_IMPLEMENTED
_AVLOU32NodeCore* RTAvloU32RemoveBestFit(PAVLOU32TREE, AVLOU32KEY, bool)
	NOT_IMPLEMENTED_CHECKED(0)
int RTAvlU32Destroy(PAVLU32TREE, PAVLU32CALLBACK, void*) NOT_IMPLEMENTED
_AVLU32NodeCore* RTAvlU32GetBestFit(PAVLU32TREE, AVLU32KEY, bool) NOT_IMPLEMENTED
int RTAvloU32DoWithAll(PAVLOU32TREE, int, PAVLOU32CALLBACK, void*) NOT_IMPLEMENTED
bool RTAvloU32Insert(PAVLOU32TREE, PAVLOU32NODECORE) NOT_IMPLEMENTED
_AVLU32NodeCore* RTAvlU32Get(PAVLU32TREE, AVLU32KEY) NOT_IMPLEMENTED
int RTAvlU32DoWithAll(PAVLU32TREE, int, PAVLU32CALLBACK, void*) NOT_IMPLEMENTED
bool RTAvlU32Insert(PAVLU32TREE, PAVLU32NODECORE) NOT_IMPLEMENTED

int IOMR3Init(PVM) NOT_IMPLEMENTED_CHECKED(0)
int IOMR3IOPortRegisterRC(PVM, PPDMDEVINS, RTIOPORT, RTUINT, RTRCPTR, RTRCPTR,
                          RTRCPTR, RTRCPTR, RTRCPTR, const char*)
	NOT_IMPLEMENTED_CHECKED(0)
int IOMR3IOPortRegisterR0(PVM, PPDMDEVINS, RTIOPORT, RTUINT, RTR0PTR,
                          RTHCUINTPTR, RTHCUINTPTR, RTHCUINTPTR, RTHCUINTPTR,
                          const char*) NOT_IMPLEMENTED_CHECKED(0)
int IOMR3MmioRegisterR0(PVM, PPDMDEVINS, RTGCPHYS, uint32_t, RTR0PTR,
                        RTHCUINTPTR, RTHCUINTPTR, RTHCUINTPTR)
	NOT_IMPLEMENTED_CHECKED(0)
int IOMR3MmioRegisterRC(PVM, PPDMDEVINS, RTGCPHYS, uint32_t, RTGCPTR, RTRCPTR,
                        RTRCPTR, RTRCPTR)
	NOT_IMPLEMENTED_CHECKED(0)

void IOMR3Relocate(PVM, RTGCINTPTR) NOT_IMPLEMENTED_CHECKED()
void IOMR3Reset(PVM) NOT_IMPLEMENTED_CHECKED()
int  IOMR3Term(PVM) NOT_IMPLEMENTED

VBOXSTRICTRC IOMInterpretOUT(PVM, PCPUMCTXCORE, PDISCPUSTATE) NOT_IMPLEMENTED
VBOXSTRICTRC IOMInterpretOUTS(PVM, PCPUMCTXCORE, PDISCPUSTATE) NOT_IMPLEMENTED
VBOXSTRICTRC IOMInterpretIN(PVM, PCPUMCTXCORE, PDISCPUSTATE) NOT_IMPLEMENTED
VBOXSTRICTRC IOMInterpretINS(PVM, PCPUMCTXCORE, PDISCPUSTATE) NOT_IMPLEMENTED


int DISInstrToStrWithReader (RTUINTPTR, DISCPUMODE, PFNDISREADBYTES, void*,
                             PDISSTATE, uint32_t*, char*, size_t) NOT_IMPLEMENTED
int DISInstrToStrEx(RTUINTPTR, DISCPUMODE, PFNDISREADBYTES, void*, uint32_t,
                    PDISSTATE, uint32_t*, char*, size_t) NOT_IMPLEMENTED

DUMMY(-1, RTFileQueryFsSizes)

DUMMY(-1, pthread_mutex_timedlock)

int PGMR3HandlerVirtualRegister(PVM, PGMVIRTHANDLERTYPE, RTGCPTR, RTGCPTR,
                                PFNPGMR3VIRTINVALIDATE, PFNPGMR3VIRTHANDLER,
                                const char*, const char*, const char*)
	NOT_IMPLEMENTED_CHECKED(0) /* XXX */
int PGMHandlerVirtualDeregister(PVM, RTGCPTR)
	NOT_IMPLEMENTED_CHECKED(0) /* XXX */

/*
 * Dummies added for storage
 */
int RTAvlrFileOffsetDestroy(PAVLRFOFFTREE, PAVLRFOFFCALLBACK, void*) NOT_IMPLEMENTED
_AVLRFOFFNodeCore* RTAvlrFileOffsetGet(PAVLRFOFFTREE, RTFOFF) NOT_IMPLEMENTED
_AVLRFOFFNodeCore* RTAvlrFileOffsetGetBestFit(PAVLRFOFFTREE, RTFOFF, bool) NOT_IMPLEMENTED
bool RTAvlrFileOffsetInsert(PAVLRFOFFTREE, PAVLRFOFFNODECORE) NOT_IMPLEMENTED
_AVLRFOFFNodeCore* RTAvlrFileOffsetRemove(PAVLRFOFFTREE, RTFOFF) NOT_IMPLEMENTED
int RTAvlrU64Destroy(PAVLRU64TREE, PAVLRU64CALLBACK, void*) NOT_IMPLEMENTED
int RTAvlrU64DoWithAll(PAVLRU64TREE, int, PAVLRU64CALLBACK, void*) NOT_IMPLEMENTED
AVLRU64NodeCore* RTAvlrU64GetBestFit(PAVLRU64TREE, AVLRU64KEY, bool) NOT_IMPLEMENTED
bool RTAvlrU64Insert(PAVLRU64TREE, PAVLRU64NODECORE) NOT_IMPLEMENTED
AVLRU64NodeCore* RTAvlrU64RangeGet(PAVLRU64TREE, AVLRU64KEY) NOT_IMPLEMENTED
AVLRU64NodeCore* RTAvlrU64RangeRemove(PAVLRU64TREE, AVLRU64KEY) NOT_IMPLEMENTED
AVLRU64NodeCore* RTAvlrU64Remove(PAVLRU64TREE, AVLRU64KEY) NOT_IMPLEMENTED
DUMMY(-1, RTLdrClose)
DUMMY(-1, RTMemDupExTag)
DUMMY(-1, rtPathRootSpecLen)
DUMMY(-1, RTPathStartsWithRoot)
DUMMY(-1, RTSocketToNative)
DUMMY(-1, RTStrCatP)
DUMMY(-1, RTTcpClientCloseEx)
DUMMY(-1, RTTcpClientConnect)
DUMMY(-1, RTTcpFlush)
DUMMY(-1, RTTcpGetLocalAddress)
DUMMY(-1, RTTcpGetPeerAddress)
DUMMY(-1, RTTcpRead)
DUMMY(-1, RTTcpReadNB)
DUMMY(-1, RTTcpSelectOne)
DUMMY(-1, RTTcpSelectOneEx)
DUMMY(-1, RTTcpSetSendCoalescing)
DUMMY(-1, RTTcpSgWrite)
DUMMY(-1, RTTcpSgWriteNB)
DUMMY(-1, RTTcpWrite)
DUMMY(-1, RTTcpWriteNB)
DUMMY(-1, RTTimeLocalExplode)

DUMMY(-1, RTSymlinkCreate)
DUMMY(-1, RTSymlinkRead)
DUMMY(-1, RTSymlinkDelete)

DUMMY(-1, RTNetIPv6PseudoChecksumEx)

CHECKED_DUMMY(0, futimes)
CHECKED_DUMMY(0, lutimes)

int __isthreaded;

int sigprocmask() { return 0; }
int _sigprocmask() { return 0; }

int PGMFlushTLB(PVMCPU, uint64_t, bool) NOT_IMPLEMENTED_CHECKED(0)

 /* seems to be needed on raw mode only */
int PGMInvalidatePage(PVMCPU, RTGCPTR) NOT_IMPLEMENTED_CHECKED(0)

int PGMHandlerPhysicalPageTempOff(PVM, RTGCPHYS, RTGCPHYS) NOT_IMPLEMENTED_CHECKED(0)

bool PGMIsLockOwner(PVM) NOT_IMPLEMENTED_CHECKED(0) /* assertion in EMRemLock */
bool IOMIsLockOwner(PVM) NOT_IMPLEMENTED_CHECKED(0) /* XXX */

int  MMHyperIsInsideArea() { return 0; } /* used by dbgfR3DisasInstrRead */
void PGMPhysReleasePageMappingLock(PVM, PPGMPAGEMAPLOCK) NOT_IMPLEMENTED_CHECKED()
} /* extern "C" */

