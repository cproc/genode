/*
 * \brief  Singlethreaded minimalistic kernel
 * \author Martin Stein
 * \author Stefan Kalkowski
 * \date   2011-10-20
 *
 * This kernel is the only code except the mode transition PIC, that runs in
 * privileged CPU mode. It has two tasks. First it initializes the process
 * 'core', enriches it with the whole identically mapped address range,
 * joins and applies it, assigns one thread to it with a userdefined
 * entrypoint (the core main thread) and starts this thread in userland.
 * Afterwards it is called each time an exception occurs in userland to do
 * a minimum of appropriate exception handling. Thus it holds a CPU context
 * for itself as for any other thread. But due to the fact that it never
 * relies on prior kernel runs this context only holds some constant pointers
 * such as SP and IP.
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <kernel/pd.h>
#include <kernel/vm.h>
#include <platform_pd.h>
#include <trustzone.h>
#include <timer.h>
#include <pic.h>
#include <map_local.h>

/* base includes */
#include <base/allocator_avl.h>
#include <unmanaged_singleton.h>
#include <base/native_types.h>

/* base-hw includes */
#include <kernel/irq.h>
#include <kernel/perf_counter.h>
using namespace Kernel;

extern Genode::Native_thread_id _main_thread_id;
extern "C" void CORE_MAIN();
extern void * _start_secondary_processors;
extern int _prog_img_beg;
extern int _prog_img_end;

Genode::Native_utcb * _main_thread_utcb;

namespace Kernel
{
	/**
	 * Return interrupt-controller singleton
	 */
	Pic * pic() { return unmanaged_singleton<Pic>(); }

	/* import Genode types */
	typedef Genode::umword_t       umword_t;
	typedef Genode::Core_thread_id Core_thread_id;
}

namespace Kernel
{
	Pd_ids * pd_ids() { return unmanaged_singleton<Pd_ids>(); }
	Thread_ids * thread_ids() { return unmanaged_singleton<Thread_ids>(); }
	Signal_context_ids * signal_context_ids() { return unmanaged_singleton<Signal_context_ids>(); }
	Signal_receiver_ids * signal_receiver_ids() { return unmanaged_singleton<Signal_receiver_ids>(); }

	Pd_pool * pd_pool() { return unmanaged_singleton<Pd_pool>(); }
	Thread_pool * thread_pool() { return unmanaged_singleton<Thread_pool>(); }
	Signal_context_pool * signal_context_pool() { return unmanaged_singleton<Signal_context_pool>(); }
	Signal_receiver_pool * signal_receiver_pool() { return unmanaged_singleton<Signal_receiver_pool>(); }

	/**
	 * Return singleton kernel-timer
	 */
	Timer * timer()
	{
		static Timer _object;
		return &_object;
	}

	/**
	 * Start a new scheduling lap
	 */
	void reset_scheduling_time(unsigned const processor_id)
	{
		unsigned const tics = timer()->ms_to_tics(USER_LAP_TIME_MS);
		timer()->start_one_shot(tics, processor_id);
	}


	/**
	 * Static kernel PD that describes core
	 */
	Pd * core_pd()
	{
		using Ttable = Genode::Translation_table;
		constexpr int tt_align  = 1 << Ttable::ALIGNM_LOG2;

		/**
		 * Dummy page slab backend allocator for bootstrapping only
		 */
		struct Simple_allocator : Genode::Core_mem_translator
		{
			Simple_allocator() { }

			int add_range(addr_t base, size_t size) { return -1; }
			int remove_range(addr_t base, size_t size) { return -1; }
			Alloc_return alloc_aligned(size_t size, void **out_addr, int align) {
				return Alloc_return::RANGE_CONFLICT; }
			Alloc_return alloc_addr(size_t size, addr_t addr) {
				return Alloc_return::RANGE_CONFLICT; }
			void   free(void *addr) {}
			size_t avail() { return 0; }
			bool valid_addr(addr_t addr) { return false; }
			bool alloc(size_t size, void **out_addr) { return false; }
			void free(void *addr, size_t) {  }
			size_t overhead(size_t size) { return 0; }
			bool need_size_for_free() const override { return false; }

			void * phys_addr(void * addr) { return addr; }
			void * virt_addr(void * addr) { return addr; }
		};

		struct Core_pd : Platform_pd, Pd
		{

bool map(addr_t from_phys, addr_t to_virt, size_t num_pages, bool io_mem)
{
	using namespace Genode;
        Translation_table *tt = Pd::translation_table();
        const Page_flags flags = Page_flags::map_core_area(io_mem);

        try {
                for (unsigned i = 0; i < 2; i++) {
                        try {
                                //Lock::Guard guard(*Kernel::core_pd()->platform_pd()->lock());

                                tt->insert_translation(to_virt, from_phys,
                                                       num_pages * get_page_size(), flags,
                                                       platform_pd()->page_slab());
                                return true;
                        } catch(Page_slab::Out_of_slabs) {
                                platform_pd()->page_slab()->alloc_slab_block();
                        }
                }
        } catch(Allocator::Out_of_memory) {
                PERR("Translation table needs to much RAM");
        } catch(...) {
                PERR("Invalid mapping %p -> %p (%zx)", (void*)from_phys, (void*)to_virt,
                         get_page_size() * num_pages);
        }
        return false;
}

			Core_pd(Ttable * tt, Genode::Page_slab * slab)
			: Platform_pd(tt, slab),
			  Pd(tt, this)
			{
				using namespace Genode;
PDBG("%u", __LINE__);
				Platform_pd::_id = Pd::id();
PDBG("%u", __LINE__);
				/* map exception vector for core */
				Kernel::mtc()->map(tt, slab);
PDBG("%u", __LINE__);
				/* map core's program image */
				addr_t start = trunc_page((addr_t)&_prog_img_beg);
				addr_t end   = round_page((addr_t)&_prog_img_end);
				map(start, start, (end-start) / get_page_size(), false);
PDBG("%u", __LINE__);
				/* map core's mmio regions */
				Native_region * r = Platform::_core_only_mmio_regions(0);
PDBG("%u", __LINE__);
				for (unsigned i = 0; r;
				     r = Platform::_core_only_mmio_regions(++i))
					map(r->base, r->base, r->size / get_page_size(), true);
PDBG("%u", __LINE__);
			}
		};

		Simple_allocator  * sa   = unmanaged_singleton<Simple_allocator>();
		Ttable            * tt   = unmanaged_singleton<Ttable, tt_align>();
		Genode::Page_slab * slab = unmanaged_singleton<Genode::Page_slab,
		                                               tt_align>(sa);
		return unmanaged_singleton<Core_pd>(tt, slab);
	}

	/**
	 * Return wether an interrupt is private to the kernel
	 *
	 * \param interrupt_id  kernel name of the targeted interrupt
	 */
	bool private_interrupt(unsigned const interrupt_id)
	{
		bool ret = 0;
		for (unsigned i = 0; i < PROCESSORS; i++) {
			ret |= interrupt_id == Timer::interrupt_id(i);
		}
		return ret;
	}
}


namespace Kernel
{
	/**
	 * Get attributes of the mode transition region in every PD
	 */
	addr_t mode_transition_base() { return mtc()->VIRT_BASE; }
	size_t mode_transition_size() { return mtc()->SIZE; }

	/**
	 * Get attributes of the kernel objects
	 */
	size_t thread_size()          { return sizeof(Thread); }
	size_t signal_context_size()  { return sizeof(Signal_context); }
	size_t signal_receiver_size() { return sizeof(Signal_receiver); }
	size_t vm_size()              { return sizeof(Vm); }
	unsigned pd_alignm_log2() { return Genode::Translation_table::ALIGNM_LOG2; }
	size_t pd_size() { return sizeof(Genode::Translation_table) + sizeof(Pd); }

	enum { STACK_SIZE = 64 * 1024 };

	/**
	 * Return lock that guards all kernel data against concurrent access
	 */
	Lock & data_lock()
	{
		static Lock s;
		return s;
	}

	addr_t   core_tt_base;
	unsigned core_pd_id;
}


/**
 * Enable kernel-entry assembly to get an exclusive stack at every processor
 */
unsigned kernel_stack_size = Kernel::STACK_SIZE;
char     kernel_stack[PROCESSORS][Kernel::STACK_SIZE]
         __attribute__((aligned(16)));


/**
 * Setup kernel enviroment before activating secondary processors
 */
extern "C" void init_kernel_uniprocessor()
{
	/************************************************************************
	 ** As atomic operations are broken in physical mode on some platforms **
	 ** we must avoid the use of 'cmpxchg' by now (includes not using any  **
	 ** local static objects.                                              **
	 ************************************************************************/
PDBG("%u", __LINE__);
	/* calculate in advance as needed later when data writes aren't allowed */
	core_tt_base = (addr_t) core_pd()->translation_table();
PDBG("%u", __LINE__);
	core_pd_id   = core_pd()->id();
PDBG("%u", __LINE__);
	/* initialize all processor objects */
	processor_pool();
PDBG("%u", __LINE__);
	/* go multiprocessor mode */
	Processor::start_secondary_processors(&_start_secondary_processors);
PDBG("%u", __LINE__);
}

/**
 * Setup kernel enviroment after activating secondary processors
 */
extern "C" void init_kernel_multiprocessor()
{
	/***********************************************************************
	 ** As updates on a cached kernel lock might not be visible to        **
	 ** processors that have not enabled caches, we can't synchronize the **
	 ** activation of MMU and caches. Hence we must avoid write access to **
	 ** kernel data by now.                                               **
	 ***********************************************************************/
PDBG("%u", __LINE__);
	/* synchronize data view of all processors */
	Processor::invalidate_data_caches();
PDBG("%u", __LINE__);
	Processor::invalidate_instr_caches();
PDBG("%u", __LINE__);
	Processor::invalidate_control_flow_predictions();
PDBG("%u", __LINE__);
	Processor::data_synchronization_barrier();
PDBG("%u", __LINE__);
	/* initialize processor in physical mode */
	Processor::init_phys_kernel();
PDBG("%u", __LINE__);
	/* switch to core address space */
	Processor::init_virt_kernel(core_tt_base, core_pd_id);
PDBG("%u", __LINE__);
	/************************************
	 ** Now it's safe to use 'cmpxchg' **
	 ************************************/

	Lock::Guard guard(data_lock());
PDBG("%u", __LINE__);
	/*******************************************
	 ** Now it's save to write to kernel data **
	 *******************************************/

	/*
	 * TrustZone initialization code
	 *
	 * FIXME This is a plattform specific feature
	 */
	init_trustzone(pic());
PDBG("%u", __LINE__);
	/*
	 * Enable performance counter
	 *
	 * FIXME This is an optional processor specific feature
	 */
	perf_counter()->enable();
PDBG("%u", __LINE__);
	/* initialize interrupt controller */
	pic()->init_processor_local();
	unsigned const processor_id = Processor::executing_id();
	pic()->unmask(Timer::interrupt_id(processor_id), processor_id);
PDBG("%u", __LINE__);
	/* as primary processor create the core main thread */
	if (Processor::primary_id() == processor_id)
	{
		/* get stack memory that fullfills the constraints for core stacks */
		enum {
			STACK_ALIGNM = 1 << Genode::CORE_STACK_ALIGNM_LOG2,
			STACK_SIZE   = DEFAULT_STACK_SIZE,
		};
		static_assert(STACK_SIZE <= STACK_ALIGNM - sizeof(Core_thread_id),
		              "stack size does not fit stack alignment of core");
		static char s[STACK_SIZE] __attribute__((aligned(STACK_ALIGNM)));

		/* provide thread ident at the aligned base of the stack */
		*(Core_thread_id *)s = 0;
PDBG("%u", __LINE__);
		/* start thread with stack pointer at the top of stack */
		static Native_utcb utcb;
PDBG("%u", __LINE__);
		static Thread t(Priority::MAX, "core");
PDBG("%u", __LINE__);
		_main_thread_id = t.id();
		_main_thread_utcb = &utcb;
		_main_thread_utcb->start_info()->init(t.id(), Genode::Native_capability());
		t.ip = (addr_t)CORE_MAIN;;
		t.sp = (addr_t)s + STACK_SIZE;
		t.init(processor_pool()->processor(processor_id), core_pd(), &utcb, 1);

		/* initialize interrupt objects */
		static Genode::uint8_t _irqs[Pic::MAX_INTERRUPT_ID * sizeof(Irq)];
		for (unsigned i = 0; i < Pic::MAX_INTERRUPT_ID; i++) {
			if (private_interrupt(i)) { continue; }
			new (&_irqs[i * sizeof(Irq)]) Irq(i);
		}
		/* kernel initialization finished */
		Genode::printf("kernel initialized\n");
	}
	reset_scheduling_time(processor_id);
}


/**
 * Main routine of every kernel pass
 */
extern "C" void kernel()
{
	/* ensure that no other processor accesses kernel data while we do */
	data_lock().lock();

	/* determine local processor scheduler */
	unsigned const processor_id = Processor::executing_id();
	Processor * const processor = processor_pool()->processor(processor_id);
	Processor_scheduler * const scheduler = processor->scheduler();

	/*
	 * Request the current processor occupant without any update. While this
	 * processor was outside the kernel, another processor may have changed the
	 * scheduling of the local activities in a way that an update would return
	 * an occupant other than that whose exception caused the kernel entry.
	 */
	Processor_client * const old_occupant = scheduler->occupant();
	old_occupant->exception(processor_id);

	/*
	 * The processor local as well as remote exception-handling may have
	 * changed the scheduling of the local activities. Hence we must update the
	 * processor occupant.
	 */
	Processor_client * const new_occupant = scheduler->update_occupant();
	if (old_occupant != new_occupant) { reset_scheduling_time(processor_id); }
	new_occupant->proceed(processor_id);
}


Kernel::Mode_transition_control * Kernel::mtc()
{
	/* create singleton processor context for kernel */
	Cpu_context * const cpu_context = unmanaged_singleton<Cpu_context>();

	/* initialize mode transition page */
	return unmanaged_singleton<Mode_transition_control>(cpu_context);
}


Kernel::Cpu_context::Cpu_context()
{
	_init(STACK_SIZE);
	sp = (addr_t)kernel_stack;
	ip = (addr_t)kernel;
	core_pd()->admit(this);
}
