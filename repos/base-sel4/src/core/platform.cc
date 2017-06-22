/*
 * \brief   Platform interface implementation
 * \author  Norman Feske
 * \date    2015-05-01
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/sleep.h>
#include <base/thread.h>
#include <base/log.h>

/* core includes */
#include <boot_modules.h>
#include <platform.h>
#include <map_local.h>
#include <cnode.h>
#include <untyped_memory.h>

/* base-internal includes */
#include <base/internal/globals.h>
#include <base/internal/stack_area.h>

using namespace Genode;

static bool const verbose_boot_info = true;

/**
 * Platform object is set if object is currently in construction.
 * Avoids deadlocks due to nested calls of Platform() constructor caused by
 * platform_specific(). May happen if meta data allocator of phys_alloc runs
 * out of memory.
 */
static Platform * platform_in_construction = nullptr;

/*
 * Memory-layout information provided by the linker script
 */

/* virtual address range consumed by core's program image */
extern unsigned _prog_img_beg, _prog_img_end;


/****************************************
 ** Support for core memory management **
 ****************************************/

bool Mapped_mem_allocator::_map_local(addr_t virt_addr, addr_t phys_addr,
                                      unsigned size)
{
	if (platform_in_construction)
		Genode::warning("need physical memory, but Platform object not constructed yet");

	size_t const num_pages = size / get_page_size();

	Untyped_memory::convert_to_page_frames(phys_addr, num_pages);

	return map_local(phys_addr, virt_addr, num_pages, platform_in_construction);
}


bool Mapped_mem_allocator::_unmap_local(addr_t virt_addr, addr_t phys_addr,
                                        unsigned size)
{
	if (!unmap_local(virt_addr, size / get_page_size()))
		return false;

	Untyped_memory::convert_to_untyped_frames(phys_addr, size);

	return true;
}


/************************
 ** Platform interface **
 ************************/

void Platform::_init_unused_phys_alloc()
{
	/* the lower physical ram is kept by the kernel and not usable to us */
	_unused_phys_alloc.add_range(0x100000, 0UL - 0x100000);
}


void Platform::_init_allocators()
{
	/* interrupt allocator */
	_irq_alloc.add_range(0, 256);

	/*
	 * XXX allocate intermediate CNodes for organizing the untyped pages here
	 */

	/* turn remaining untyped memory ranges into untyped pages */
	_initial_untyped_pool.turn_into_untyped_object(Core_cspace::TOP_CNODE_UNTYPED_IDX,
		[&] (addr_t const phys, addr_t const size, bool const device) {
			/* register to physical or iomem memory allocator */

			addr_t const phys_addr = trunc_page(phys);
			size_t const phys_size = round_page(phys - phys_addr + size);

			if (device)
				_io_mem_alloc.add_range(phys_addr, phys_size);
			else
				_core_mem_alloc.phys_alloc()->add_range(phys_addr, phys_size);

			_unused_phys_alloc.remove_range(phys_addr, phys_size);
		});

	/*
	 * From this point on, we can no longer create kernel objects from the
	 * '_initial_untyped_pool' because the pool is empty.
	 */

	/* core's maximum virtual memory area */
	_unused_virt_alloc.add_range(_vm_base, _vm_size);

	/* remove core image from core's virtual address allocator */
	addr_t const modules_start  = reinterpret_cast<addr_t>(&_boot_modules_binaries_begin);
	addr_t const core_virt_beg  = trunc_page((addr_t)&_prog_img_beg),
	             core_virt_end  = round_page((addr_t)&_prog_img_end);
	addr_t const image_elf_size = core_virt_end - core_virt_beg;

	_unused_virt_alloc.remove_range(core_virt_beg, image_elf_size);
	_core_mem_alloc.virt_alloc()->add_range(modules_start, core_virt_end - modules_start);

	/* remove initial IPC buffer from core's virtual address allocator */
	seL4_BootInfo const &bi = sel4_boot_info();
	addr_t const core_ipc_buffer = reinterpret_cast<addr_t>(bi.ipcBuffer);
	addr_t const core_ipc_bsize  = 4096;
	_unused_virt_alloc.remove_range(core_ipc_buffer, core_ipc_bsize);

	/* remove sel4_boot_info page from core's virtual address allocator */
	addr_t const boot_info_page = reinterpret_cast<addr_t>(&bi);
	addr_t const boot_info_size = 4096 + bi.extraLen;
	_unused_virt_alloc.remove_range(boot_info_page, boot_info_size);

	/* preserve stack area in core's virtual address space */
	_unused_virt_alloc.remove_range(stack_area_virtual_base(),
	                                stack_area_virtual_size());

	if (verbose_boot_info) {
		typedef Hex_range<addr_t> Hex_range;
		log("virtual address layout of core:");
		log(" overall    ", Hex_range(_vm_base, _vm_size));
		log(" core image ", Hex_range(core_virt_beg, image_elf_size));
		log(" ipc buffer ", Hex_range(core_ipc_buffer, core_ipc_bsize));
		log(" boot_info  ", Hex_range(boot_info_page, boot_info_size));
		log(" stack area ", Hex_range(stack_area_virtual_base(),
		                              stack_area_virtual_size()));
	}
}


void Platform::_switch_to_core_cspace()
{
	Cnode_base const initial_cspace(Cap_sel(seL4_CapInitThreadCNode),
	                                CONFIG_WORD_SIZE);

	/* copy initial selectors to core's CNode */
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapInitThreadTCB));
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapInitThreadVSpace));
	_core_cnode.move(initial_cspace, Cnode_index(seL4_CapIRQControl)); /* cannot be copied */
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapASIDControl));
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapInitThreadASIDPool));
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapIOPort));
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapBootInfoFrame));
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapInitThreadIPCBuffer));
	_core_cnode.copy(initial_cspace, Cnode_index(seL4_CapDomain));

	/* replace seL4_CapInitThreadCNode with new top-level CNode */
	_core_cnode.copy(initial_cspace, Cnode_index(Core_cspace::top_cnode_sel()),
	                                 Cnode_index(seL4_CapInitThreadCNode));

	/* copy untyped memory selectors to core's CNode */
	seL4_BootInfo const &bi = sel4_boot_info();

	/*
	 * We have to move (not copy) the selectors for the initial untyped ranges
	 * because some of them are already populated with kernel objects allocated
	 * via '_initial_untyped_pool'. For such an untyped memory range, the
	 * attempt to copy its selector would result in the following error:
	 *
	 *   <<seL4: Error deriving cap for CNode Copy operation.>>
	 */
	for (unsigned sel = bi.untyped.start; sel < bi.untyped.end; sel++)
		_core_cnode.move(initial_cspace, Cnode_index(sel));

	/* move selectors of core image */
	addr_t const modules_start = reinterpret_cast<addr_t>(&_boot_modules_binaries_begin);
	addr_t const modules_end   = reinterpret_cast<addr_t>(&_boot_modules_binaries_end);
	addr_t virt_addr = (addr_t)(&_prog_img_beg);

	for (unsigned sel = bi.userImageFrames.start;
	     sel < bi.userImageFrames.end;
	     sel++, virt_addr += get_page_size()) {

		/* remove mapping to boot modules, no access required within core */
		if (modules_start <= virt_addr && virt_addr < modules_end) {
			long err = _unmap_page_frame(Cap_sel(sel));
			if (err != seL4_NoError)
				error("unmapping boot modules ", Hex(virt_addr), " error=", err);
		}

		/* insert cap for core image */
		_core_cnode.move(initial_cspace, Cnode_index(sel));
	}

	/* copy statically created CNode selectors to core's CNode */
	_core_cnode.copy(initial_cspace, Cnode_index(Core_cspace::top_cnode_sel()));
	_core_cnode.copy(initial_cspace, Cnode_index(Core_cspace::core_pad_cnode_sel()));
	_core_cnode.copy(initial_cspace, Cnode_index(Core_cspace::core_cnode_sel()));
	_core_cnode.copy(initial_cspace, Cnode_index(Core_cspace::phys_cnode_sel()));

	/*
	 * Construct CNode hierarchy of core's CSpace
	 */

	/* insert 3rd-level core CNode into 2nd-level core-pad CNode */
	_core_pad_cnode.copy(initial_cspace, Cnode_index(Core_cspace::core_cnode_sel()),
	                                     Cnode_index(0));

	/* insert 2nd-level core-pad CNode into 1st-level CNode */
	_top_cnode.copy(initial_cspace, Cnode_index(Core_cspace::core_pad_cnode_sel()),
	                                Cnode_index(Core_cspace::TOP_CNODE_CORE_IDX));

	/* insert 2nd-level phys-mem CNode into 1st-level CNode */
	_top_cnode.copy(initial_cspace, Cnode_index(Core_cspace::phys_cnode_sel()),
	                                Cnode_index(Core_cspace::TOP_CNODE_PHYS_IDX));

	/* insert 2nd-level untyped-pages CNode into 1st-level CNode */
	_top_cnode.copy(initial_cspace, Cnode_index(Core_cspace::untyped_cnode_sel()),
	                                Cnode_index(Core_cspace::TOP_CNODE_UNTYPED_IDX));

	/* activate core's CSpace */
	{
		seL4_CapData_t null_data = { { 0 } };
		seL4_CapData_t const guard = seL4_CapData_Guard_new(0, CONFIG_WORD_SIZE - 32);

		int const ret = seL4_TCB_SetSpace(seL4_CapInitThreadTCB,
		                                  seL4_CapNull, /* fault_ep */
		                                  Core_cspace::top_cnode_sel(),
		                                  guard,
		                                  seL4_CapInitThreadPD, null_data);

		if (ret != seL4_NoError)
			error(__FUNCTION__, ": seL4_TCB_SetSpace returned ", ret);
	}
}


Cap_sel Platform::_init_asid_pool()
{
	return Cap_sel(seL4_CapInitThreadASIDPool);
}


void Platform::_init_rom_modules()
{
	seL4_BootInfo const &bi = sel4_boot_info();

	/*
	 * Slab allocator for allocating 'Rom_module' meta data.
	 */
	static long slab_block[4096];
	static Tslab<Rom_module, sizeof(slab_block)>
		rom_module_slab(core_mem_alloc(), (Genode::Slab_block *)slab_block);

	/*
	 * Allocate unused range of phys CNode address space where to make the
	 * boot modules available.
	 */
	void *out_ptr = nullptr;
	size_t const modules_size = (addr_t)&_boot_modules_binaries_end
	                          - (addr_t)&_boot_modules_binaries_begin + 1;

	Range_allocator::Alloc_return const alloc_ret =
		_unused_phys_alloc.alloc_aligned(modules_size, &out_ptr, get_page_size_log2());

	if (alloc_ret.error()) {
		error("could not reserve phys CNode space for boot modules");
		struct Init_rom_modules_failed { };
		throw Init_rom_modules_failed();
	}

	/*
	 * Calculate frame frame selector used to back the boot modules
	 */
	addr_t const unused_range_start      = (addr_t)out_ptr;
	addr_t const unused_first_frame_sel  = unused_range_start >> get_page_size_log2();
	addr_t const modules_start           = (addr_t)&_boot_modules_binaries_begin;
	addr_t const modules_core_offset     = modules_start
	                                     - (addr_t)&_prog_img_beg;
	addr_t const modules_first_frame_sel = bi.userImageFrames.start
	                                     + (modules_core_offset >> get_page_size_log2());

	Boot_modules_header const *header = &_boot_modules_headers_begin;
	for (; header < &_boot_modules_headers_end; header++) {

		/* offset relative to first module */
		addr_t const module_offset        = header->base - modules_start;
		addr_t const module_offset_frames = module_offset >> get_page_size_log2();
		size_t const module_size          = round_page(header->size);
		addr_t const module_frame_sel     = modules_first_frame_sel
		                                  + module_offset_frames;
		size_t const module_num_frames    = module_size >> get_page_size_log2();

		/*
		 * Destination frame within phys CNode
		 */
		addr_t const dst_frame = unused_first_frame_sel + module_offset_frames;

		/*
		 * Install the module's frame selectors into phys CNode
		 */
		Cnode_base const initial_cspace(Cap_sel(seL4_CapInitThreadCNode), 32);
		for (unsigned i = 0; i < module_num_frames; i++)
			_phys_cnode.move(initial_cspace, Cnode_index(module_frame_sel + i),
			                                 Cnode_index(dst_frame + i));

		log("boot module '", (char const *)header->name, "' "
		    "(", header->size, " bytes)");

		/*
		 * Register ROM module, the base address refers to location of the
		 * ROM module within the phys CNode address space.
		 */
		Rom_module * rom_module = new (rom_module_slab)
			Rom_module(dst_frame << get_page_size_log2(), header->size,
			           (const char*)header->name);

		_rom_fs.insert(rom_module);
	}
}


Platform::Platform()
:

	_io_mem_alloc(core_mem_alloc()), _io_port_alloc(core_mem_alloc()),
	_irq_alloc(core_mem_alloc()),
	_unused_phys_alloc(core_mem_alloc()),
	_unused_virt_alloc(core_mem_alloc()),
	_init_unused_phys_alloc_done((_init_unused_phys_alloc(), true)),
	_vm_base(0x2000), /* 2nd page is used as IPC buffer of main thread */
	_vm_size(3*1024*1024*1024UL - _vm_base), /* use the lower 3GiB */
	_init_sel4_ipc_buffer_done((init_sel4_ipc_buffer(), true)),
	_switch_to_core_cspace_done((_switch_to_core_cspace(), true)),
	_core_page_table_registry(_core_page_table_registry_alloc),
	_init_core_page_table_registry_done((_init_core_page_table_registry(), true)),
	_init_allocators_done((_init_allocators(), true)),
	_core_vm_space(Cap_sel(seL4_CapInitThreadPD),
	               _core_sel_alloc,
	               _phys_alloc,
	               _top_cnode,
	               _core_cnode,
	               _phys_cnode,
	               Core_cspace::CORE_VM_ID,
	               _core_page_table_registry,
	               "core")
{
	platform_in_construction = this;

	/* create notification object for Genode::Lock used by this first thread */
	Cap_sel lock_sel (INITIAL_SEL_LOCK);
	Cap_sel core_sel = _core_sel_alloc.alloc();

	{
		addr_t       const phys_addr = Untyped_memory::alloc_page(*ram_alloc());
		seL4_Untyped const service   = Untyped_memory::untyped_sel(phys_addr).value();
		create<Notification_kobj>(service, core_cnode().sel(), core_sel);
	}

	/* mint a copy of the notification object with badge of lock_sel */
	_core_cnode.mint(_core_cnode, core_sel, lock_sel);

	/* test signal/wakeup once */
	seL4_Word sender;
	seL4_Signal(lock_sel.value());
	seL4_Wait(lock_sel.value(), &sender);

	ASSERT(sender == INITIAL_SEL_LOCK);

	/* back stack area with page tables */
	enum { MAX_CORE_THREADS = 32 };
	_core_vm_space.unsynchronized_alloc_page_tables(stack_area_virtual_base(),
	                                                stack_virtual_size() *
	                                                MAX_CORE_THREADS);

	/* add some minor virtual region for dynamic usage by core */
	addr_t const virt_size = 32 * 1024 * 1024;
	void * virt_ptr = nullptr;
	if (_unused_virt_alloc.alloc(virt_size, &virt_ptr)) {

		addr_t const virt_addr = (addr_t)virt_ptr;

		/* add to available virtual region of core */
		_core_mem_alloc.virt_alloc()->add_range(virt_addr, virt_size);

		/* back region by page tables */
		_core_vm_space.unsynchronized_alloc_page_tables(virt_addr, virt_size);
	}

	/* I/O port allocator (only meaningful for x86) */
	_io_port_alloc.add_range(0, 0x10000);

	/*
	 * Log statistics about allocator initialization
	 */
	if (verbose_boot_info) {
		log(":phys_alloc:       ", *_core_mem_alloc.phys_alloc());
		log(":unused_phys_alloc:",  _unused_phys_alloc);
		log(":unused_virt_alloc:",  _unused_virt_alloc);
		log(":virt_alloc:       ", *_core_mem_alloc.virt_alloc());
		log(":io_mem_alloc:     ",  _io_mem_alloc);
	}

	_init_rom_modules();

	platform_in_construction = nullptr;
}


unsigned Platform::alloc_core_rcv_sel()
{
	Cap_sel rcv_sel = _core_sel_alloc.alloc();

	seL4_SetCapReceivePath(_core_cnode.sel().value(), rcv_sel.value(),
	                       _core_cnode.size_log2());

	return rcv_sel.value();
}


void Platform::reset_sel(unsigned sel)
{
	_core_cnode.remove(Cap_sel(sel));
}


void Platform::wait_for_exit()
{
	sleep_forever();
}

