/*
 * \brief  Genode/Nova specific VirtualBox SUPLib supplements
 * \author Alexander Boettcher
 * \author Norman Feske
 * \author Christian Helmuth
 */

/*
 * Copyright (C) 2006-2013 Oracle Corporation
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* Genode includes */
#include <util/bit_allocator.h>
#include <base/log.h>
#include <base/semaphore.h>
#include <util/flex_iterator.h>
#include <rom_session/connection.h>
#include <timer_session/connection.h>
#include <base/attached_rom_dataspace.h>
#include <base/attached_ram_dataspace.h>
#include <trace/timestamp.h>

#include <vmm/vcpu_thread.h>
#include <vmm/vcpu_dispatcher.h>

/* NOVA includes that come with Genode */
#include <nova/syscalls.h>

/* Genode's VirtualBox includes */
#include "EMInternal.h" /* enable access to em.s.* */
#include "vmm.h"
#include "vcpu.h"
#include "vcpu_svm.h"
#include "vcpu_vmx.h"

/* libc memory allocator */
#include <libc_mem_alloc.h>

#include "mm.h"


extern "C" bool PGMUnmapMemoryGenode(void *, ::size_t);


/*
 * Tracking required to fulfill VMM allocation requests of VM memory
 */
enum {
	CHUNKID_PAGE_START = 1,
	CHUNKID_PAGE_END   = 2,
	CHUNKID_START      = CHUNKID_PAGE_END + 1,

	ONE_PAGE_SIZE      = 4096,
	PAGES_SUPERPAGE    = GMM_CHUNK_SIZE / ONE_PAGE_SIZE,
	HANDY_PAGES        = PAGES_SUPERPAGE * (CHUNKID_PAGE_END - CHUNKID_PAGE_START + 1),

	MAX_VM_MEMORY      = 16ULL * 1024 * 1024 * 1024, /* 16 Gb */
	MAX_CHUNK_IDS      = MAX_VM_MEMORY / GMM_CHUNK_SIZE,
};

typedef Genode::Bit_allocator<HANDY_PAGES>   Page_ids;
typedef Genode::Bit_allocator<MAX_CHUNK_IDS> Chunk_ids;
typedef Genode::Bit_array<PAGES_SUPERPAGE>    Free_ids;

static Page_ids  page_ids;
static Chunk_ids chunk_ids;


static Sub_rm_connection &vm_memory(Genode::addr_t vm_size = 0)
{
	/* memory used by the VM in any order as the VMM asks for allocations */
	static Sub_rm_connection vm_memory(genode_env(), vm_size);
	return vm_memory;
}


/*
 * VCPU handling
 */

static Genode::List<Vcpu_handler> &vcpu_handler_list()
{
	static Genode::List<Vcpu_handler> _inst;
	return _inst;
}


static Vcpu_handler *lookup_vcpu_handler(unsigned int cpu_id)
{
	for (Vcpu_handler *vcpu_handler = vcpu_handler_list().first();
	     vcpu_handler;
	     vcpu_handler = vcpu_handler->next())
		if (vcpu_handler->cpu_id() == cpu_id)
			return vcpu_handler;

	return 0;
}


HRESULT genode_setup_machine(ComObjPtr<Machine> machine)
{
	ULONG memory_vbox;
	HRESULT rc = machine->COMGETTER(MemorySize)(&memory_vbox);
	if (FAILED(rc))
		return rc;

	/*
	 * Extra memory because of:
	 * - first chunkid (0) can't be used (VBox don't like chunkid 0)
	 * - second chunkid (1..2) is reserved for handy pages allocation
	 * - another chunkid is used additional for handy pages but as large page
	 */
	vm_memory(memory_vbox * 1024 * 1024 + (CHUNKID_START + 1) * GMM_CHUNK_SIZE);
	return genode_check_memory_config(machine);
};


/* Genode specific function */

Nova::Hip &hip_rom()
{
	static Genode::Attached_rom_dataspace hip_rom(genode_env(),
	                                              "hypervisor_info_page");
	return *hip_rom.local_addr<Nova::Hip>();
}


void SUPR3QueryHWACCLonGenodeSupport(VM * pVM)
{
	try {
		pVM->hm.s.svm.fSupported = hip_rom().has_feature_svm();
		pVM->hm.s.vmx.fSupported = hip_rom().has_feature_vmx();

		if (hip_rom().has_feature_svm() || hip_rom().has_feature_vmx()) {
			Genode::log("Using ", hip_rom().has_feature_svm() ? "SVM" : "VMX",
			            " virtualization extension.");
			return;
		}
	} catch (...) { /* if we get an exception let hardware support off */ }

	Genode::warning("No virtualization hardware acceleration available");
}


/* VirtualBox SUPLib interface */
int SUPR3QueryVTxSupported(void) { return VINF_SUCCESS; }


int SUPR3CallVMMR0Fast(PVMR0 pVMR0, unsigned uOperation, VMCPUID idCpu)
{
	switch (uOperation) {
	case SUP_VMMR0_DO_HM_RUN:
		Vcpu_handler *vcpu_handler = lookup_vcpu_handler(idCpu);
		Assert(vcpu_handler);
		return vcpu_handler->run_hw(pVMR0);
	}
	return VERR_INTERNAL_ERROR;
}

int SUPR3PageAllocEx(::size_t cPages, uint32_t fFlags, void **ppvPages,
                     PRTR0PTR pR0Ptr, PSUPPAGE paPages)
{
	Assert(ppvPages);
	Assert(!fFlags);

	Genode::log(__func__, " cPages ", cPages, " flags=", Genode::Hex(fFlags),
	            " r3=", ppvPages, " r0=", pR0Ptr);

	using Genode::Attached_ram_dataspace;
	Attached_ram_dataspace * ds = new Attached_ram_dataspace(genode_env().ram(),
	                                                         genode_env().rm(),
	                                                         cPages * ONE_PAGE_SIZE);

	Genode::addr_t const vmm_local = reinterpret_cast<Genode::addr_t>(ds->local_addr<void>());

	*ppvPages = ds->local_addr<void>();
	if (pR0Ptr)
		*pR0Ptr = vmm_local;

	Genode::log(__func__, " cPages ", cPages, " alloc=", *ppvPages, " done");

	if (!paPages)
		return VINF_SUCCESS;

	for (unsigned iPage = 0; iPage < cPages; iPage++)
	{
		paPages[iPage].uReserved = 0;
		paPages[iPage].Phys = vmm_local + iPage * ONE_PAGE_SIZE;
	}

	return VINF_SUCCESS;
}

enum { MAX_TRACKING = 4 };
static struct {
	Free_ids free;
	unsigned freed;
	unsigned chunkid;
} track_free[MAX_TRACKING];

static void partial_free_large_page(unsigned chunkid, unsigned page_id)
{
	unsigned pos = 0;

	/* lookup if already exist */
	for (; pos < MAX_TRACKING; pos++)
	{
		if (track_free[pos].chunkid == chunkid)
			break;
	}

	/* if not exist find free place */
	if (pos >= MAX_TRACKING) {
		for (int i = 0; i < MAX_TRACKING; i++) {
			if (track_free[i].chunkid)
				continue;

			track_free[i].chunkid = chunkid;
			track_free[i].freed   = 0;
			pos = i;
			break;
		}

		/* too many chunkids in use ? */
		Assert (pos < MAX_TRACKING);
		if (pos >= MAX_TRACKING)
			return;
	}

	try {
		/* mark as in use */
		track_free[pos].free.set(page_id, 1);
		track_free[pos].freed += 1;

		if (track_free[pos].freed >= 512) {
			/* slow ? optimize ? XXX */
			for (unsigned i = 0; i < 512; i++) {
				if (!track_free[pos].free.get(i, 1))
					throw 1;
				track_free[pos].free.clear(i, 1);
			}

			track_free[pos].chunkid = 0;
			track_free[pos].freed   = 0;

			chunk_ids.free(chunkid);
		}
	} catch (...) {
		Genode::error(__func__," ", __LINE__, " allocation failed ", pos, ":",
		              chunkid, ":", page_id);
		throw;
	}
}

int SUPR3CallVMMR0Ex(PVMR0 pVMR0, VMCPUID idCpu, unsigned uOperation,
                     uint64_t u64Arg, PSUPVMMR0REQHDR pReqHdr)
{

	switch (uOperation) {

	case VMMR0_DO_GVMM_CREATE_VM:
		genode_VMMR0_DO_GVMM_CREATE_VM(pReqHdr);

		/* reserve lower chunk ids */
		try {
			for (unsigned i = 0; i < CHUNKID_START; i++) {
				unsigned chunkid = chunk_ids.alloc();
				AssertMsg(chunkid == i, ("chunkid %u != %u i\n", chunkid, i));
			}
		} catch (...) {
			Genode::error(__func__," ", __LINE__, " allocation failed");
			throw;
		}

		return VINF_SUCCESS;

	case VMMR0_DO_GVMM_REGISTER_VMCPU:
		genode_VMMR0_DO_GVMM_REGISTER_VMCPU(pVMR0, idCpu);
		return VINF_SUCCESS;

	case VMMR0_DO_GVMM_SCHED_HALT:
	{
		const uint64_t u64NowGip = RTTimeNanoTS();
		const uint64_t ns_diff = u64Arg > u64NowGip ? u64Arg - u64NowGip : 0;

		if (!ns_diff)
			return VINF_SUCCESS;

		uint64_t const tsc_offset = genode_cpu_hz() * ns_diff / (1000*1000*1000);
		uint64_t const tsc_abs    = Genode::Trace::timestamp() + tsc_offset;

		using namespace Genode;

		if (ns_diff > RT_NS_1SEC)
			warning(" more than 1 sec vcpu halt ", ns_diff, " ns");

		Vcpu_handler *vcpu_handler = lookup_vcpu_handler(idCpu);
		Assert(vcpu_handler);
		vcpu_handler->halt(tsc_abs);

		return VINF_SUCCESS;
	}

	case VMMR0_DO_GVMM_SCHED_WAKE_UP:
	{
		Vcpu_handler *vcpu_handler = lookup_vcpu_handler(idCpu);
		Assert(vcpu_handler);

		/* don't wake the currently running thread again */
		if (vcpu_handler->utcb() == Genode::Thread::myself()->utcb())
			return VINF_SUCCESS;

		vcpu_handler->wake_up();
		return VINF_SUCCESS;
	}

	/* called by 'vmR3HaltGlobal1Halt' */
	case VMMR0_DO_GVMM_SCHED_POLL:
		return VINF_SUCCESS;

	case VMMR0_DO_VMMR0_INIT:
		SUPR3QueryHWACCLonGenodeSupport(reinterpret_cast<VM *>(pVMR0));
		return VINF_SUCCESS;

	case VMMR0_DO_GVMM_DESTROY_VM:
	case VMMR0_DO_VMMR0_TERM:
	case VMMR0_DO_HM_SETUP_VM:
		return VINF_SUCCESS;

	case VMMR0_DO_HM_ENABLE:
		return VINF_SUCCESS;

	case VMMR0_DO_GVMM_SCHED_POKE:
	{
		Vcpu_handler *vcpu_handler = lookup_vcpu_handler(idCpu);
		Assert(vcpu_handler);
		if (vcpu_handler)
			vcpu_handler->recall(vcpu_handler_list().first());
		return VINF_SUCCESS;
	}
	case VMMR0_DO_GMM_ALLOCATE_PAGES:
	{
		/*
		 * VMM is asking for some host virtual memory pages without
		 * allocating the backing store actually. The backing store allocation
		 * takes place via VMMR0_DO_GMM_MAP_UNMAP_CHUNK. The assignment of the
		 * guest addresses is known at this point.
		 */

		static_assert(PGM_HANDY_PAGES * ONE_PAGE_SIZE == GMM_CHUNK_SIZE,
		              "Don't do that - you're going to waste tons of memory");
		Assert(pReqHdr->u32Magic == SUPVMMR0REQHDR_MAGIC);

		/* XXX VMM/VMMR0/GMMR0.cpp check there XXX */

		PGMMALLOCATEPAGESREQ req = reinterpret_cast<PGMMALLOCATEPAGESREQ>(pReqHdr);

		for (unsigned i = 0; i < req->cPages; i++) {
			RTHCPHYS guest_addr = req->aPages[i].HCPhysGCPhys;
			unsigned long page_idx = 0;
			unsigned long chunk_id = 0;

			try {
				page_idx = page_ids.alloc();
				chunk_id = CHUNKID_PAGE_START + page_idx / PAGES_SUPERPAGE;
			} catch (...) {
				Genode::error(__func__," ", __LINE__, " allocation failed");
				throw;
			}

			Assert (page_idx <= GMM_PAGEID_IDX_MASK);

			req->aPages[i].idPage = (chunk_id << GMM_CHUNKID_SHIFT) | page_idx;
			req->aPages[i].HCPhysGCPhys = vm_memory().local_addr((chunk_id * GMM_CHUNK_SIZE) | (page_idx * ONE_PAGE_SIZE));
			Assert(vm_memory().contains(req->aPages[i].HCPhysGCPhys));

			#if 0
			Genode::log("cPages ", Genode::Hex(req->cPages), " "
			            "chunkID=", req->aPages[i].idPage >> GMM_CHUNKID_SHIFT, " "
			            "pageIDX=", req->aPages[i].idPage & GMM_PAGEID_IDX_MASK, " "
			            "idPage=", Genode::Hex(req->aPages[i].idPage), " "
			            "GCPhys=", Genode::Hex(guest_addr), " "
			            "HCPhys=", Genode::Hex(req->aPages[i].HCPhysGCPhys), " "
			            "(", Genode::Hex(chunk_id * GMM_CHUNK_SIZE), " "
			            "| ", Genode::Hex(page_idx * ONE_PAGE_SIZE), ") pageidx=", page_idx, " "
			            "start_vm=", vm_memory().local_addr(0));
			#endif

		}

		return VINF_SUCCESS;
	}
	case VMMR0_DO_GMM_MAP_UNMAP_CHUNK:
	{
		PGMMMAPUNMAPCHUNKREQ req = reinterpret_cast<PGMMMAPUNMAPCHUNKREQ>(pReqHdr);

		Assert(pReqHdr->u32Magic == SUPVMMR0REQHDR_MAGIC);
		Assert(req->idChunkUnmap == NIL_GMM_CHUNKID);
		Assert(req->idChunkMap   != NIL_GMM_CHUNKID);

		Genode::Ram_dataspace_capability ds = genode_env().ram().alloc(GMM_CHUNK_SIZE);
		Genode::addr_t local_addr_offset = req->idChunkMap << GMM_CHUNK_SHIFT;

		enum { OFFSET_DS = 0, USE_LOCAL_ADDR = true };
		Genode::addr_t to = vm_memory().attach(ds, GMM_CHUNK_SIZE, OFFSET_DS,
		                                       USE_LOCAL_ADDR, local_addr_offset);
		Assert(to == vm_memory().local_addr(local_addr_offset));

		req->pvR3 = reinterpret_cast<RTR3PTR>(to);

		return VINF_SUCCESS;
	}
	case VMMR0_DO_GMM_QUERY_MEM_STATS:
	{
		PGMMMEMSTATSREQ req = reinterpret_cast<PGMMMEMSTATSREQ>(pReqHdr);
		req->cAllocPages = 0;
		req->cMaxPages = 0;
		req->cBalloonedPages = 0;
		return VINF_SUCCESS;
	}
	case VMMR0_DO_PGM_ALLOCATE_HANDY_PAGES:
	{
		/*
		 * VMM is asking for some host virtual memory pages without
		 * allocating the backing store actually. The backing store allocation
		 * takes place via VMMR0_DO_GMM_MAP_UNMAP_CHUNK. The assignment of the
		 * guest addresses to these host pages is unknown at this point.
		 */

		PVM pVM = reinterpret_cast<PVM>(pVMR0);

		/* based on PGMR0PhysAllocateHandyPages() in VMM/VMMR0/PGMR0.cpp - start */
		uint32_t iFirst = pVM->pgm.s.cHandyPages;
		uint32_t cPages = RT_ELEMENTS(pVM->pgm.s.aHandyPages) - iFirst;
		uint32_t cPagesToUpdate = cPages;
		uint32_t cPagesToAlloc  = cPages;
		/* based on PGMR0PhysAllocateHandyPages() in VMM/VMMR0/PGMR0.cpp - end */

		/* based on GMMR0AllocateHandyPages in VMM/VMMR0/GMMR0.cpp - start */
		unsigned iPage = 0;
		for (; iPage < cPagesToUpdate; iPage++)
		{
			AssertMsgReturn(    (    pVM->pgm.s.aHandyPages[iFirst + iPage].HCPhysGCPhys <= GMM_GCPHYS_LAST
			                     && !(pVM->pgm.s.aHandyPages[iFirst + iPage].HCPhysGCPhys & PAGE_OFFSET_MASK))
			                ||  pVM->pgm.s.aHandyPages[iFirst + iPage].HCPhysGCPhys == NIL_RTHCPHYS
			                ||  pVM->pgm.s.aHandyPages[iFirst + iPage].HCPhysGCPhys == GMM_GCPHYS_UNSHAREABLE,
			                ("#%#x: %RHp\n", iFirst + iPage, pVM->pgm.s.aHandyPages[iFirst + iPage].HCPhysGCPhys),
			                VERR_INVALID_PARAMETER);
			AssertMsgReturn(    pVM->pgm.s.aHandyPages[iFirst + iPage].idPage <= GMM_PAGEID_LAST
			                /*||  pVM->pgm.s.aHandyPages[iFirst + iPage].idPage == NIL_GMM_PAGEID*/,
			                ("#%#x: %#x\n", iFirst + iPage, pVM->pgm.s.aHandyPages[iFirst + iPage].idPage), VERR_INVALID_PARAMETER);
			AssertMsgReturn(    pVM->pgm.s.aHandyPages[iFirst + iPage].idPage <= GMM_PAGEID_LAST
			                /*||  pVM->pgm.s.aHandyPages[iFirst + iPage].idSharedPage == NIL_GMM_PAGEID*/,
			                ("#%#x: %#x\n", iFirst + iPage, pVM->pgm.s.aHandyPages[iFirst + iPage].idSharedPage), VERR_INVALID_PARAMETER);
		}

		for (; iPage < cPagesToAlloc; iPage++)
		{
			AssertMsgReturn(pVM->pgm.s.aHandyPages[iFirst + iPage].HCPhysGCPhys == NIL_RTHCPHYS,   ("#%#x: %RHp\n", iFirst + iPage, pVM->pgm.s.aHandyPages[iFirst + iPage].HCPhysGCPhys), VERR_INVALID_PARAMETER);
			AssertMsgReturn(pVM->pgm.s.aHandyPages[iFirst + iPage].idPage       == NIL_GMM_PAGEID, ("#%#x: %#x\n", iFirst + iPage, pVM->pgm.s.aHandyPages[iFirst + iPage].idPage),        VERR_INVALID_PARAMETER);
			AssertMsgReturn(pVM->pgm.s.aHandyPages[iFirst + iPage].idSharedPage == NIL_GMM_PAGEID, ("#%#x: %#x\n", iFirst + iPage, pVM->pgm.s.aHandyPages[iFirst + iPage].idSharedPage),  VERR_INVALID_PARAMETER);
		}

		unsigned chunkid = 0;

		try {
			chunkid = chunk_ids.alloc();
		} catch (...) {
			Genode::error(__func__," ", __LINE__, " allocation failed");
			throw;
		}

		if (cPagesToAlloc != GMM_CHUNK_SIZE / ONE_PAGE_SIZE)
			Vmm::log("special chunkid=", chunkid, " "
			         "toupdate=", cPagesToUpdate, " "
			         "toalloc=", cPagesToAlloc, " "
			         "virt=", Genode::Hex(vm_memory().local_addr(chunkid << GMM_CHUNK_SHIFT)));

		for (unsigned i = 0; i < cPagesToUpdate; i++) {
			if (pVM->pgm.s.aHandyPages[iFirst + i].idPage != NIL_GMM_PAGEID)
			{
				pVM->pgm.s.aHandyPages[iFirst + i].idPage = NIL_GMM_PAGEID;
				pVM->pgm.s.aHandyPages[iFirst + i].HCPhysGCPhys = NIL_RTHCPHYS;
			}

			if (pVM->pgm.s.aHandyPages[iFirst + i].idSharedPage != NIL_GMM_PAGEID)
				AssertMsgReturn(false, ("%s %u - not implemented", __func__, __LINE__), VERR_GENERAL_FAILURE);
		}

		for (unsigned i = 0; i < cPagesToAlloc; i++)
		{
			Assert(pVM->pgm.s.aHandyPages[iFirst + i].HCPhysGCPhys  == NIL_RTHCPHYS);
			Assert(pVM->pgm.s.aHandyPages[iFirst + i].idPage        == NIL_GMM_PAGEID);
			Assert(pVM->pgm.s.aHandyPages[iFirst + i].idSharedPage  == NIL_GMM_PAGEID);
		}

		for (unsigned i = 0; i < cPagesToUpdate; i++) {
			unsigned reverse = i; //cPagesToUpdate - 1 - i;
			Assert (pVM->pgm.s.aHandyPages[iFirst + i].HCPhysGCPhys == NIL_RTHCPHYS);
			{
				pVM->pgm.s.aHandyPages[iFirst + i].idPage = (chunkid << GMM_CHUNKID_SHIFT) | (iFirst + reverse);
				pVM->pgm.s.aHandyPages[iFirst + i].idSharedPage = NIL_GMM_PAGEID;

				pVM->pgm.s.aHandyPages[iFirst + i].HCPhysGCPhys = vm_memory().local_addr((chunkid << GMM_CHUNK_SHIFT) | ((iFirst + reverse) * ONE_PAGE_SIZE));
			}
		}
		/* based on GMMR0AllocateHandyPages in VMM/VMMR0/GMMR0.cpp - end */

		/* based on PGMR0PhysAllocateHandyPages() in VMM/VMMR0/PGMR0.cpp - start */
		pVM->pgm.s.cHandyPages = RT_ELEMENTS(pVM->pgm.s.aHandyPages);

		for (uint32_t i = 0; i < RT_ELEMENTS(pVM->pgm.s.aHandyPages); i++)
		{
			Assert(pVM->pgm.s.aHandyPages[i].idPage != NIL_GMM_PAGEID);
			Assert(pVM->pgm.s.aHandyPages[i].idPage <= GMM_PAGEID_LAST);
			Assert(pVM->pgm.s.aHandyPages[i].idSharedPage == NIL_GMM_PAGEID);
			Assert(pVM->pgm.s.aHandyPages[i].HCPhysGCPhys != NIL_RTHCPHYS);
			Assert(!(pVM->pgm.s.aHandyPages[i].HCPhysGCPhys & ~X86_PTE_PAE_PG_MASK));
		}
		/* based on PGMR0PhysAllocateHandyPages() in VMM/VMMR0/PGMR0.cpp - end */

		return VINF_SUCCESS;
	}
	case VMMR0_DO_PGM_ALLOCATE_LARGE_HANDY_PAGE:
	{
		/*
		 * VMM is asking for some host large virtual memory pages without
		 * allocating the backing store actually. The backing store allocation
		 * takes place via VMMR0_DO_GMM_MAP_UNMAP_CHUNK. The assignment of the
		 * guest addresses to these host pages is unknown at this point.
		 */

		PVM pVM = reinterpret_cast<PVM>(pVMR0);

		Assert(pVM);
		Assert(pVM->pgm.s.cLargeHandyPages == 0);

		try {
			unsigned chunkid = chunk_ids.alloc();

			pVM->pgm.s.aLargeHandyPage[0].idPage = (chunkid << GMM_CHUNKID_SHIFT);
			pVM->pgm.s.aLargeHandyPage[0].HCPhysGCPhys = vm_memory().local_addr(chunkid << GMM_CHUNK_SHIFT);

			pVM->pgm.s.cLargeHandyPages = 1;
		} catch (...) {
			Genode::error(__func__," ", __LINE__, " allocation failed");
			throw;
		}

		return VINF_SUCCESS;
	}
	case VMMR0_DO_GMM_BALLOONED_PAGES:
	case VMMR0_DO_GMM_RESET_SHARED_MODULES:
	case VMMR0_DO_PGM_FLUSH_HANDY_PAGES:
	{
		PVM    const pVM   = reinterpret_cast<PVM>(pVMR0);
		PVMCPU const pVCpu = &pVM->aCpus[idCpu];

		/* if not in VM shutdown - complain - bug ahead */
		if (pVCpu->em.s.enmState != EMSTATE_TERMINATING)
			Genode::error("unexpected call of type ", uOperation, ", "
			              "em state=", (int)pVCpu->em.s.enmState);

		return VINF_SUCCESS;
	}
	case VMMR0_DO_GMM_FREE_PAGES:
	{
		if (u64Arg)
			return VERR_INVALID_PARAMETER;

		PVM pVM = reinterpret_cast<PVM>(pVMR0);
		PGMMFREEPAGESREQ pReq = reinterpret_cast<PGMMFREEPAGESREQ>(pReqHdr);

		AssertPtrReturn(pVM, VERR_INVALID_POINTER);
		AssertPtrReturn(pReq, VERR_INVALID_POINTER);
		AssertMsgReturn(pReq->Hdr.cbReq >= RT_UOFFSETOF(GMMFREEPAGESREQ, aPages[0]),
		                ("%#x < %#x\n", pReq->Hdr.cbReq, RT_UOFFSETOF(GMMFREEPAGESREQ, aPages[0])),
		                VERR_INVALID_PARAMETER);
		AssertMsgReturn(pReq->Hdr.cbReq == RT_UOFFSETOF(GMMFREEPAGESREQ, aPages[pReq->cPages]),
		                ("%#x != %#x\n", pReq->Hdr.cbReq, RT_UOFFSETOF(GMMFREEPAGESREQ, aPages[pReq->cPages])),
		                VERR_INVALID_PARAMETER);

		uint32_t cPages = pReq->cPages;
		PGMMFREEPAGEDESC paPages = &pReq->aPages[0];
		GMMACCOUNT enmAccount = pReq->enmAccount;

		AssertPtrReturn(paPages, VERR_INVALID_PARAMETER);
		AssertMsgReturn(enmAccount > GMMACCOUNT_INVALID && enmAccount < GMMACCOUNT_END, ("%d\n", enmAccount), VERR_INVALID_PARAMETER);
		AssertMsgReturn(cPages > 0 && cPages < RT_BIT(32 - PAGE_SHIFT), ("%#x\n", cPages), VERR_INVALID_PARAMETER);

		for (unsigned iPage = 0; iPage < cPages; iPage++)
			AssertMsgReturn(    paPages[iPage].idPage <= GMM_PAGEID_LAST
			                /*||  paPages[iPage].idPage == NIL_GMM_PAGEID*/,
			                ("#%#x: %#x\n", iPage, paPages[iPage].idPage), VERR_INVALID_PARAMETER);

		for (uint32_t last_chunk = ~0U, iPage = 0; iPage < cPages; iPage++)
		{
			uint32_t const idPage   = paPages[iPage].idPage;
			uint32_t const page_idx = idPage & GMM_PAGEID_IDX_MASK;
			uint32_t const chunkid  = idPage >> GMM_CHUNKID_SHIFT;

			if (last_chunk != chunkid) {
				/* revoke mapping from guest VM */
				void * vmm_local = reinterpret_cast<void *>(vm_memory().local_addr(chunkid << GMM_CHUNK_SHIFT));
				PGMUnmapMemoryGenode(vmm_local, GMM_CHUNK_SIZE);

				last_chunk = chunkid;
			}

			if (CHUNKID_PAGE_START <= chunkid && chunkid <= CHUNKID_PAGE_END) {
				try {
					page_ids.free((chunkid - CHUNKID_PAGE_START) * PAGES_SUPERPAGE + page_idx);
				} catch (...) {
					Genode::error(__func__," ", __LINE__, " clearing failed");
					throw;
				}
			}

			partial_free_large_page(chunkid, page_idx);

			paPages[iPage].idPage = NIL_GMM_PAGEID;
		}

		return VINF_SUCCESS;
	}
	case VMMR0_DO_GMM_INITIAL_RESERVATION:
		Genode::log("VMMR0_DO_GMM_INITIAL_RESERVATION called");
		return VINF_SUCCESS;
	case VMMR0_DO_GMM_UPDATE_RESERVATION:
		Genode::log("VMMR0_DO_GMM_UPDATE_RESERVATION called");
		return VINF_SUCCESS;
	default:
		Genode::error("SUPR3CallVMMR0Ex: unhandled uOperation ", uOperation,
		              " ", (int)VMMR0_DO_PGM_ALLOCATE_HANDY_PAGES, " ",
		              (int)VMMR0_DO_GMM_QUERY_MEM_STATS);
		return VERR_GENERAL_FAILURE;
	}
}


/**
 * Various support stuff - base-nova specific.
 */
uint64_t genode_cpu_hz()
{
	static uint64_t cpu_freq = 0;

	if (!cpu_freq) {
		try {
			cpu_freq = hip_rom().tsc_freq * 1000;
		} catch (...) {
			Genode::error("could not read out CPU frequency");
			Genode::Lock lock;
			lock.lock();
		}
	}

	return cpu_freq;
}


void genode_update_tsc(void (*update_func)(void), unsigned long update_us)
{
	using namespace Genode;
	using namespace Nova;

	enum { TSC_FACTOR = 1000ULL };

	Genode::addr_t sem = Thread::myself()->native_thread().exc_pt_sel + Nova::SM_SEL_EC;
	unsigned long tsc_khz = (genode_cpu_hz() / 1000) / TSC_FACTOR;

	Trace::Timestamp us_64 = update_us;

	for (;;) {
		update_func();

		Trace::Timestamp now = Trace::timestamp();

		/* block until timeout fires or it gets canceled */
		unsigned long long tsc_absolute = now + us_64 * tsc_khz;
		Genode::uint8_t res = sm_ctrl(sem, SEMAPHORE_DOWN, tsc_absolute);
		if (res != Nova::NOVA_OK && res != Nova::NOVA_TIMEOUT)
			nova_die();
	}
}


bool PGMUnmapMemoryGenode(void * vmm_local, ::size_t size)
{
	Assert(vmm_local);

	using namespace Genode;

	Flexpage_iterator fli(reinterpret_cast<addr_t>(vmm_local), size, 0, ~0UL, 0);

	Flexpage revoke_page = fli.page();
	while (revoke_page.valid()) {
		Assert(revoke_page.log2_order >= 12);
		Assert(!(((1UL << revoke_page.log2_order) - 1) & revoke_page.addr));

		using namespace Nova;

		Rights const revoke_rwx(true, true, true);
		Crd crd = Mem_crd(revoke_page.addr >> 12, revoke_page.log2_order - 12,
		                  revoke_rwx);
		revoke(crd, false);

		/* request next page(s) to be revoked */
		revoke_page = fli.page();
	}

	return true;
}


extern "C" void pthread_yield(void)
{
	Nova::ec_ctrl(Nova::EC_YIELD);
}


void *operator new (__SIZE_TYPE__ size, int log2_align)
{
	static Libc::Mem_alloc_impl heap(genode_env().rm(), genode_env().ram());
	return heap.alloc(size, log2_align);
}


bool create_emt_vcpu(pthread_t * pthread, ::size_t stack,
                     const pthread_attr_t *attr,
                     void *(*start_routine)(void *), void *arg,
                     Genode::Cpu_session * cpu_session,
                     Genode::Affinity::Location location,
                     unsigned int cpu_id, const char * name)
{
	if (!hip_rom().has_feature_vmx() && !hip_rom().has_feature_svm())
		return false;

	static Genode::Pd_connection pd_vcpus(genode_env(), "VM");

	Vcpu_handler *vcpu_handler = 0;

	if (hip_rom().has_feature_vmx())
		vcpu_handler = new (0x10) Vcpu_handler_vmx(genode_env(),
		                                           stack, attr, start_routine,
		                                           arg, cpu_session, location,
		                                           cpu_id, name, pd_vcpus);

	if (hip_rom().has_feature_svm())
		vcpu_handler = new (0x10) Vcpu_handler_svm(genode_env(),
		                                           stack, attr, start_routine,
		                                           arg, cpu_session, location,
		                                           cpu_id, name, pd_vcpus);

	Assert(!(reinterpret_cast<unsigned long>(vcpu_handler) & 0xf));

	vcpu_handler_list().insert(vcpu_handler);

	*pthread = vcpu_handler;
	return true;
}
