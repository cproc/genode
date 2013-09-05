/*
 * \brief  Singlethreaded minimalistic kernel
 * \author Martin Stein
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

/* Genode includes */
#include <base/signal.h>
#include <cpu/cpu_state.h>
#include <base/thread_state.h>

/* core includes */
#include <platform_thread.h>
#include <platform_pd.h>
#include <tlb.h>
#include <trustzone.h>

/* base-hw includes */
#include <singleton.h>

using namespace Kernel;

/* get core configuration */
extern Genode::Native_utcb * _main_utcb;
extern int _kernel_stack_high;
extern "C" void CORE_MAIN();

/* get structure of mode transition PIC */
extern int _mode_transition_begin;
extern int _mode_transition_end;
extern int _mt_user_entry_pic;
extern int _mon_vm_entry;
extern Genode::addr_t _mt_client_context_ptr;
extern Genode::addr_t _mt_master_context_begin;
extern Genode::addr_t _mt_master_context_end;

namespace Kernel
{
	/* import Genode types */
	typedef Genode::Thread_state Thread_state;
	typedef Genode::umword_t umword_t;

	class Schedule_context;

	/**
	 * Controls the mode transition code
	 *
	 * The code that switches between kernel/user mode must not exceed the
	 * smallest page size supported by the MMU. The Code must be position
	 * independent. This code has to be mapped in every PD, to ensure
	 * appropriate kernel invokation on CPU interrupts.
	 * This class controls the settings like kernel, user, and VM states
	 * that are handled by the mode transition PIC.
	 */
	struct Mode_transition_control
	{
		enum {
			SIZE_LOG2 = Tlb::MIN_PAGE_SIZE_LOG2,
			SIZE = 1 << SIZE_LOG2,
			VIRT_BASE = Cpu::EXCEPTION_ENTRY,
			VIRT_END = VIRT_BASE + SIZE,
			ALIGNM_LOG2 = SIZE_LOG2,
		};

		addr_t const _virt_user_entry;

		/**
		 * Constructor
		 */
		Mode_transition_control() :
			_virt_user_entry(VIRT_BASE + ((addr_t)&_mt_user_entry_pic -
			                 (addr_t)&_mode_transition_begin))
		{
			/* check if mode transition PIC fits into aligned region */
			addr_t const pic_begin = (addr_t)&_mode_transition_begin;
			addr_t const pic_end = (addr_t)&_mode_transition_end;
			size_t const pic_size = pic_end - pic_begin;
			assert(pic_size <= SIZE);

			/* check if kernel context fits into the mode transition */
			addr_t const kc_begin = (addr_t)&_mt_master_context_begin;
			addr_t const kc_end = (addr_t)&_mt_master_context_end;
			size_t const kc_size = kc_end - kc_begin;
			assert(sizeof(Cpu::Context) <= kc_size);
		}

		/**
		 * Fetch next kernelmode context
		 */
		void fetch_master_context(Cpu::Context * const c) {
			memcpy(&_mt_master_context_begin, c, sizeof(Cpu::Context)); }

		/**
		 * Page aligned physical base of the mode transition PIC
		 */
		addr_t phys_base() { return (addr_t)&_mode_transition_begin; }

		/**
		 * Jump to the usermode entry PIC
		 */
		void virt_user_entry() {
			((void(*)(void))_virt_user_entry)(); }
	};


	/**
	 * Static mode transition control
	 */
	static Mode_transition_control * mtc()
	{ return unsynchronized_singleton<Mode_transition_control>(); }

	/**
	 * Kernel object that represents a Genode PD
	 */
	class Pd : public Object<Pd, MAX_PDS>
	{
		Tlb * const         _tlb;
		Platform_pd * const _platform_pd;

		/* keep ready memory for size aligned extra costs at construction */
		enum { EXTRA_SPACE_SIZE = 2*Tlb::MAX_COSTS_PER_TRANSLATION };
		char _extra_space[EXTRA_SPACE_SIZE];

		public:

			/**
			 * Constructor
			 */
			Pd(Tlb * const t, Platform_pd * const platform_pd)
			: _tlb(t), _platform_pd(platform_pd)
			{
				/* try to add translation for mode transition region */
				Page_flags::access_t const flags = Page_flags::mode_transition();
				unsigned const slog2 =
					tlb()->insert_translation(mtc()->VIRT_BASE,
					                          mtc()->phys_base(),
					                          mtc()->SIZE_LOG2, flags);

				/* extra space needed to translate mode transition region */
				if (slog2)
				{
					/* Get size aligned extra space */
					addr_t const es = (addr_t)&_extra_space;
					addr_t const es_end = es + sizeof(_extra_space);
					addr_t const aligned_es = (es_end - (1<<slog2)) &
					                          ~((1<<slog2)-1);
					addr_t const aligned_es_end = aligned_es + (1<<slog2);

					/* check attributes of aligned extra space */
					assert(aligned_es >= es && aligned_es_end <= es_end)

					/* translate mode transition region globally */
					tlb()->insert_translation(mtc()->VIRT_BASE,
					                          mtc()->phys_base(),
					                          mtc()->SIZE_LOG2, flags,
					                          (void *)aligned_es);
				}
			}

			/**
			 * Add the CPU context 'c' to this PD
			 */
			void append_context(Cpu::Context * const c)
			{
				c->protection_domain(id());
				c->tlb(tlb()->base());
			}

			/***************
			 ** Accessors **
			 ***************/

			Tlb * const tlb() { return _tlb; }
			Platform_pd * const platform_pd() { return _platform_pd; }
	};
}

namespace Kernel
{
	/**
	 * Idle thread entry
	 */
	static void idle_main() { while (1) ; }


	/**
	 * Access to static kernel timer
	 */
	static Timer * timer() { static Timer _object; return &_object; }


	static void reset_lap_time()
	{
		timer()->start_one_shot(timer()->ms_to_tics(USER_LAP_TIME_MS));
	}

	/**
	 * Access to the static CPU scheduler
	 */
	static Cpu_scheduler * cpu_scheduler();


	/**
	 * Static kernel PD that describes core
	 */
	static Pd * core()
	{
		constexpr int tlb_align = 1 << Core_tlb::ALIGNM_LOG2;

		Core_tlb *core_tlb = unsynchronized_singleton<Core_tlb, tlb_align>();
		Pd       *pd       = unsynchronized_singleton<Pd>(core_tlb, nullptr);
		return pd;
	}


	/**
	 * Get core attributes
	 */
	unsigned core_id() { return core()->id(); }


	class Thread;

	void handle_pagefault(Thread * const);
	void handle_syscall(Thread * const);
	void handle_interrupt(void);
	void handle_invalid_excpt(void);
}


void Kernel::Thread::_schedule()
{
	cpu_scheduler()->insert(this);
	_state = SCHEDULED;
}


void Kernel::Thread::pause()
{
	assert(_state == AWAIT_RESUMPTION || _state == SCHEDULED);
	cpu_scheduler()->remove(this);
	_state = AWAIT_RESUMPTION;
}


void Kernel::Thread::stop()
{
	cpu_scheduler()->remove(this);
	_state = AWAIT_START;
}


void Kernel::Thread::request_and_wait(Thread * const dest, size_t const size) {
	Ipc_node::send_request_await_reply(dest, phys_utcb()->base(),
	                                   size, phys_utcb()->base(),
	                                   phys_utcb()->size()); }


void Kernel::Thread::wait_for_request() {
	Ipc_node::await_request(phys_utcb()->base(),
	                        phys_utcb()->size()); }


void Kernel::Thread::reply(size_t const size, bool const await_request)
{
	Ipc_node::send_reply(phys_utcb()->base(), size);
	if (await_request)
		Ipc_node::await_request(phys_utcb()->base(),
		                        phys_utcb()->size());
	else user_arg_0(0);
}


void Kernel::Thread::await_signal(Kernel::Signal_receiver * receiver)
{
	cpu_scheduler()->remove(this);
	_state = AWAIT_SIGNAL;
	_signal_receiver = receiver;
}


void Kernel::Thread::received_signal()
{
	assert(_state == AWAIT_SIGNAL);
	_schedule();
}


void Kernel::Thread::_received_irq()
{
	assert(_state == AWAIT_IRQ);
	_schedule();
}


void Kernel::Thread::handle_exception()
{
	switch(cpu_exception) {
	case SUPERVISOR_CALL:
		handle_syscall(this);
		return;
	case PREFETCH_ABORT:
	case DATA_ABORT:
		handle_pagefault(this);
		return;
	case INTERRUPT_REQUEST:
	case FAST_INTERRUPT_REQUEST:
		handle_interrupt();
		return;
	default:
		handle_invalid_excpt();
	}
}


void Kernel::Thread::proceed()
{
	_mt_client_context_ptr = (addr_t)static_cast<Genode::Cpu_state*>(this);
	mtc()->virt_user_entry();
}


void Kernel::Thread::_has_received(size_t const s)
{
	user_arg_0(s);
	if (_state != SCHEDULED) _schedule();
}


void Kernel::Thread::_awaits_receipt()
{
	cpu_scheduler()->remove(this);
	_state = AWAIT_IPC;
}


void Kernel::Thread::_awaits_irq()
{
	cpu_scheduler()->remove(this);
	_state = AWAIT_IRQ;
}


namespace Kernel
{
	class Signal_receiver;

	/**
	 * Specific signal type, owned by a receiver, can be triggered asynchr.
	 */
	class Signal_context : public Object<Signal_context, MAX_SIGNAL_CONTEXTS>,
	                       public Fifo<Signal_context>::Element
	{
		friend class Signal_receiver;

		Signal_receiver * const _receiver;  /* the receiver that owns us */
		unsigned const          _imprint;   /* every outgoing signals gets
		                                     * signed with this */
		unsigned                _submits;   /* accumul. undelivered submits */
		bool                    _await_ack; /* delivery ack pending */
		Thread *                _killer;    /* awaits our destruction if >0 */

		/**
		 * Utility to deliver all remaining submits
		 */
		void _deliver();

		/**
		 * Called by receiver when all submits have been delivered
		 */
		void _delivered()
		{
			_submits = 0;
			_await_ack = 1;
		}

		public:

			/**
			 * Constructor
			 */
			Signal_context(Signal_receiver * const r, unsigned const imprint) :
				_receiver(r), _imprint(imprint),
				_submits(0), _await_ack(0), _killer(0) { }

			/**
			 * Submit the signal
			 *
			 * \param n  number of submits
			 */
			void submit(unsigned const n);

			/**
			 * Acknowledge delivery of signal
			 */
			void ack();

			/**
			 * Destruct or prepare to do it at next call of 'ack'
			 *
			 * \return  wether destruction is done
			 */
			bool kill(Thread * const killer)
			{
				assert(!_killer);
				_killer = killer;
				if (_await_ack) {
					_killer->kill_signal_context_blocks();
					return 0;
				}
				this->~Signal_context();
				return 1;
			}
	};

	/**
	 * Manage signal contexts and enable threads to trigger and await them
	 */
	class Signal_receiver :
		public Object<Signal_receiver, MAX_SIGNAL_RECEIVERS>
	{
		Fifo<Thread> _listeners;
		Fifo<Signal_context> _pending_contexts;

		/**
		 * Deliver as much submitted signals to listening threads as possible
		 */
		void _listen()
		{
			while (1)
			{
				/* any pending context? */
				if (_pending_contexts.empty()) return;
				Signal_context * const c = _pending_contexts.dequeue();

				/* if there is no listener, enqueue context again and return */
				if (_listeners.empty()) {
					_pending_contexts.enqueue(c);
					return;
				}
				/* awake a listener and transmit signal info to it */
				Thread * const t = _listeners.dequeue();
				Signal::Data data((Genode::Signal_context *)c->_imprint,
				                  c->_submits);
				*(Signal::Data *)t->phys_utcb()->base() = data;
				t->received_signal();
				c->_delivered();
			}
		}

		public:

			/**
			 * Let a thread listen to our contexts
			 */
			void add_listener(Thread * const t)
			{
				t->await_signal(this);
				_listeners.enqueue(t);
				_listen();
			}

			/**
			 * Stop a thread from listening to our contexts
			 */
			void remove_listener(Thread * const t) {
				_listeners.remove(t); }

			/**
			 * If any of our contexts is pending
			 */
			bool pending() { return !_pending_contexts.empty(); }

			/**
			 * Recognize that 'c' wants to deliver
			 */
			void deliver(Signal_context * const c)
			{
				assert(c->_receiver == this);
				if (!c->is_enqueued()) _pending_contexts.enqueue(c);
				_listen();
			}
	};


	class Vm : public Object<Vm, MAX_VMS>,
	           public Schedule_context
	{
		private:

			Genode::Cpu_state_modes * const _state;
			Signal_context * const          _context;

		public:

			void * operator new (size_t, void * p) { return p; }

			/**
			 * Constructor
			 */
			Vm(Genode::Cpu_state_modes * const state,
			   Signal_context * const context)
			: _state(state), _context(context) { }


			/**************************
			 ** Vm_session interface **
			 **************************/

			void run()   { cpu_scheduler()->insert(this); }
			void pause() { cpu_scheduler()->remove(this); }


			/**********************
			 ** Schedule_context **
			 **********************/

			void handle_exception()
			{
				switch(_state->cpu_exception) {
				case Genode::Cpu_state::INTERRUPT_REQUEST:
				case Genode::Cpu_state::FAST_INTERRUPT_REQUEST:
					handle_interrupt();
					return;
				default:
					cpu_scheduler()->remove(this);
					_context->submit(1);
				}
			}

			void proceed()
			{
				/* set context pointer for mode switch */
				_mt_client_context_ptr = (addr_t)_state;

				/* jump to assembler path */
				((void(*)(void))&_mon_vm_entry)();
			}
	};


	/**
	 * Access to static CPU scheduler
	 */
	Cpu_scheduler * cpu_scheduler()
	{
		/* create idle thread */
		static char idle_stack[DEFAULT_STACK_SIZE]
		__attribute__((aligned(Cpu::DATA_ACCESS_ALIGNM)));
		static Thread idle((Platform_thread *)0);
		static bool initial = 1;
		if (initial)
		{
			/* initialize idle thread */
			void * sp;
			sp = (void *)&idle_stack[sizeof(idle_stack)/sizeof(idle_stack[0])];

			/*
			 * Idle doesn't use its UTCB pointer, thus
			 * utcb_phys = utcb_virt = 0 is save.
			 * Base-hw doesn't support multiple cores, thus
			 * cpu_no = 0 is ok. We don't use 'start' to avoid
			 * recursive call of'cpu_scheduler'.
			 */
			idle.prepare_to_start((void *)&idle_main, sp, 0, core_id(), 0, 0);
			initial = 0;
		}
		/* create scheduler with a permanent idle thread */
		static Cpu_scheduler cpu_sched(&idle);
		return &cpu_sched;
	}

	/**
	 * Get attributes of the mode transition region in every PD
	 */
	addr_t mode_transition_virt_base() { return mtc()->VIRT_BASE; }
	size_t mode_transition_size()      { return mtc()->SIZE; }

	/**
	 * Get attributes of the kernel objects
	 */
	size_t thread_size()          { return sizeof(Thread); }
	size_t pd_size()              { return sizeof(Tlb) + sizeof(Pd); }
	size_t signal_context_size()  { return sizeof(Signal_context); }
	size_t signal_receiver_size() { return sizeof(Signal_receiver); }
	unsigned pd_alignm_log2()     { return Tlb::ALIGNM_LOG2; }
	size_t vm_size()              { return sizeof(Vm); }


	/**
	 * Handle the occurence of an unknown exception
	 */
	void handle_invalid_excpt() { assert(0); }


	/**
	 * Handle an interrupt request
	 */
	void handle_interrupt()
	{
		/* determine handling for specific interrupt */
		unsigned irq;
		if (pic()->take_request(irq))
		{
			switch (irq) {

			case Timer::IRQ: {

				cpu_scheduler()->yield();
				timer()->clear_interrupt();
				reset_lap_time();
				break; }

			default: {

				Irq_receiver * const o = Irq_receiver::receiver(irq);
				assert(o);
				o->receive_irq(irq);
				break; }
			}
		}
		/* disengage interrupt controller from IRQ */
		pic()->finish_request();
	}


	/**
	 * Handle an usermode pagefault
	 *
	 * \param user  thread that has caused the pagefault
	 */
	void handle_pagefault(Thread * const user)
	{
		/* check out cause and attributes of abort */
		addr_t virt_addr      = 0;
		bool   write          = 0;
		bool   is_transl_miss = user->translation_miss(virt_addr, write);
		assert(is_transl_miss);

		/* the user might be able to resolve the pagefault */
		user->pagefault(virt_addr, write);
	}


	/**
	 * Handle request of an unknown signal type
	 */
	void handle_invalid_syscall(Thread * const) { assert(0); }


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_new_pd(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* create TLB and PD */
		void * dst = (void *)user->user_arg_1();
		Tlb * const tlb = new (dst) Tlb();
		dst = (void *)((addr_t)dst + sizeof(Tlb));
		Pd * const pd = new (dst) Pd(tlb, (Platform_pd *)user->user_arg_2());

		/* return success */
		user->user_arg_0(pd->id());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_new_thread(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* dispatch arguments */
		Syscall_arg const arg1 = user->user_arg_1();
		Syscall_arg const arg2 = user->user_arg_2();

		/* create thread */
		Thread * const t = new ((void *)arg1)
		Thread((Platform_thread *)arg2);

		/* return thread ID */
		user->user_arg_0((Syscall_ret)t->id());
	}

	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_delete_thread(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* get targeted thread */
		unsigned thread_id = (unsigned)user->user_arg_1();
		Thread * const thread = Thread::pool()->object(thread_id);
		assert(thread);

		/* destroy thread */
		thread->~Thread();
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_start_thread(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* dispatch arguments */
		Platform_thread * pt = (Platform_thread *)user->user_arg_1();
		void * const ip = (void *)user->user_arg_2();
		void * const sp = (void *)user->user_arg_3();
		unsigned const cpu = (unsigned)user->user_arg_4();

		/* get targeted thread */
		Thread * const t = Thread::pool()->object(pt->id());
		assert(t);

		/* start thread */
		t->start(ip, sp, cpu, pt->pd_id(), pt->phys_utcb(), pt->virt_utcb());

		/* return software TLB that the thread is assigned to */
		Pd::Pool * const pp = Pd::pool();
		Pd * const pd = pp->object(t->pd_id());
		assert(pd);
		user->user_arg_0((Syscall_ret)pd->tlb());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_pause_thread(Thread * const user)
	{
		unsigned const tid = user->user_arg_1();

		/* shortcut for a thread to pause itself */
		if (!tid) {
			user->pause();
			user->user_arg_0(0);
			return;
		}

		/* get targeted thread and check permissions */
		Thread * const t = Thread::pool()->object(tid);
		assert(t && (user->pd_id() == core_id() || user==t));

		/* pause targeted thread */
		t->pause();
		user->user_arg_0(0);
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_resume_thread(Thread * const user)
	{
		/* get targeted thread */
		Thread * const t = Thread::pool()->object(user->user_arg_1());
		assert(t);

		/* check permissions */
		assert(user->pd_id() == core_id() || user->pd_id() == t->pd_id());

		/* resume targeted thread */
		user->user_arg_0(t->resume());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_resume_faulter(Thread * const user)
	{
		/* get targeted thread */
		Thread * const t = Thread::pool()->object(user->user_arg_1());
		assert(t);

		/* check permissions */
		assert(user->pd_id() == core_id() || user->pd_id() == t->pd_id());

		/*
		 * Writeback the TLB entry that resolves the fault.
		 * This is a substitution for write-through-flagging
		 * the memory that holds the TLB data, because the latter
		 * is not feasible in core space.
		 */
		Cpu::tlb_insertions();

		/* resume targeted thread */
		t->resume();
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_yield_thread(Thread * const user)
	{
		/* get targeted thread */
		Thread * const t = Thread::pool()->object(user->user_arg_1());

		/* invoke kernel object */
		if (t) t->resume();
		cpu_scheduler()->yield();
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_current_thread_id(Thread * const user)
	{ user->user_arg_0((Syscall_ret)user->id()); }


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_get_thread(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* get target */
		unsigned const tid = (unsigned)user->user_arg_1();
		Thread * t;

		/* user targets a thread by ID */
		if (tid) {
			t = Thread::pool()->object(tid);
			assert(t);

		/* user targets itself */
		} else t = user;

		/* return target platform thread */
		user->user_arg_0((Syscall_ret)t->platform_thread());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_wait_for_request(Thread * const user)
	{
		user->wait_for_request();
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_request_and_wait(Thread * const user)
	{
		/* get IPC receiver */
		Thread * const t = Thread::pool()->object(user->user_arg_1());
		assert(t);

		/* do IPC */
		user->request_and_wait(t, (size_t)user->user_arg_2());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_reply(Thread * const user) {
		user->reply((size_t)user->user_arg_1(), (bool)user->user_arg_2()); }


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_set_pager(Thread * const user)
	{
		/* assert preconditions */
		assert(user->pd_id() == core_id());

		/* get faulter and pager thread */
		Thread * const p = Thread::pool()->object(user->user_arg_1());
		Thread * const f = Thread::pool()->object(user->user_arg_2());
		assert(p && f);

		/* assign pager */
		f->pager(p);
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_update_pd(Thread * const user)
	{
		assert(user->pd_id() == core_id());
		Cpu::flush_tlb_by_pid(user->user_arg_1());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_update_region(Thread * const user)
	{
		assert(user->pd_id() == core_id());

		/* FIXME we don't handle instruction caches by now */
		Cpu::flush_data_cache_by_virt_region((addr_t)user->user_arg_1(),
		                                     (size_t)user->user_arg_2());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_allocate_irq(Thread * const user)
	{
		assert(user->pd_id() == core_id());
		unsigned irq = user->user_arg_1();
		user->user_arg_0(user->allocate_irq(irq));
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_free_irq(Thread * const user)
	{
		assert(user->pd_id() == core_id());
		unsigned irq = user->user_arg_1();
		user->user_arg_0(user->free_irq(irq));
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_await_irq(Thread * const user)
	{
		assert(user->pd_id() == core_id());
		user->await_irq();
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_print_char(Thread * const user)
	{
		Genode::printf("%c", (char)user->user_arg_1());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_read_thread_state(Thread * const user)
	{
		assert(user->pd_id() == core_id());
		Thread * const t = Thread::pool()->object(user->user_arg_1());
		if (!t) PDBG("Targeted thread unknown");
		Thread_state * const ts = (Thread_state *)user->phys_utcb()->base();
		t->Cpu::Context::read_cpu_state(ts);
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_write_thread_state(Thread * const user)
	{
		assert(user->pd_id() == core_id());
		Thread * const t = Thread::pool()->object(user->user_arg_1());
		if (!t) PDBG("Targeted thread unknown");
		Thread_state * const ts = (Thread_state *)user->phys_utcb()->base();
		t->Cpu::Context::write_cpu_state(ts);
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_new_signal_receiver(Thread * const user)
	{
			/* check permissions */
			assert(user->pd_id() == core_id());

			/* create receiver */
			void * dst = (void *)user->user_arg_1();
			Signal_receiver * const r = new (dst) Signal_receiver();

			/* return success */
			user->user_arg_0(r->id());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_new_signal_context(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* lookup receiver */
		unsigned rid = user->user_arg_2();
		Signal_receiver * const r = Signal_receiver::pool()->object(rid);
		assert(r);

		/* create context */
		void * dst = (void *)user->user_arg_1();
		unsigned imprint = user->user_arg_3();
		Signal_context * const c = new (dst) Signal_context(r, imprint);

		/* return success */
		user->user_arg_0(c->id());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_await_signal(Thread * const user)
	{
		/* lookup receiver */
		unsigned rid = user->user_arg_2();
		Signal_receiver * const r = Signal_receiver::pool()->object(rid);
		assert(r);

		/* let user listen to receiver */
		r->add_listener(user);
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_signal_pending(Thread * const user)
	{
		/* lookup receiver */
		unsigned rid = user->user_arg_2();
		Signal_receiver * const r = Signal_receiver::pool()->object(rid);
		assert(r);

		/* set return value */
		user->user_arg_0(r->pending());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_submit_signal(Thread * const user)
	{
		/* lookup context */
		Signal_context * const c =
			Signal_context::pool()->object(user->user_arg_1());
		if(!c) {
			PDBG("invalid signal-context capability");
			return;
		}
		/* trigger signal at context */
		c->submit(user->user_arg_2());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_ack_signal(Thread * const user)
	{
		Signal_context * const c =
			Signal_context::pool()->object(user->user_arg_1());
		assert(c);
		c->ack();
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_kill_signal_context(Thread * const user)
	{
		Signal_context * const c =
			Signal_context::pool()->object(user->user_arg_1());
		assert(c);
		user->user_arg_0(c->kill(user));
	}

	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_new_vm(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* dispatch arguments */
		void * const allocator = (void * const)user->user_arg_1();
		Genode::Cpu_state_modes * const state =
			(Genode::Cpu_state_modes * const)user->user_arg_2();
		Signal_context * const context =
			Signal_context::pool()->object(user->user_arg_3());
		assert(context);

		/* create vm */
		Vm * const vm = new (allocator) Vm(state, context);

		/* return vm id */
		user->user_arg_0((Syscall_ret)vm->id());
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_run_vm(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* get targeted vm via its id */
		Vm * const vm = Vm::pool()->object(user->user_arg_1());
		assert(vm);

		/* run targeted vm */
		vm->run();
	}


	/**
	 * Do specific syscall for 'user', for details see 'syscall.h'
	 */
	void do_pause_vm(Thread * const user)
	{
		/* check permissions */
		assert(user->pd_id() == core_id());

		/* get targeted vm via its id */
		Vm * const vm = Vm::pool()->object(user->user_arg_1());
		assert(vm);

		/* pause targeted vm */
		vm->pause();
	}


	/**
	 * Handle a syscall request
	 *
	 * \param user  thread that called the syscall
	 */
	void handle_syscall(Thread * const user)
	{
		switch (user->user_arg_0())
		{
		case NEW_THREAD:          do_new_thread(user); return;
		case DELETE_THREAD:       do_delete_thread(user); return;
		case START_THREAD:        do_start_thread(user); return;
		case PAUSE_THREAD:        do_pause_thread(user); return;
		case RESUME_THREAD:       do_resume_thread(user); return;
		case RESUME_FAULTER:      do_resume_faulter(user); return;
		case GET_THREAD:          do_get_thread(user); return;
		case CURRENT_THREAD_ID:   do_current_thread_id(user); return;
		case YIELD_THREAD:        do_yield_thread(user); return;
		case READ_THREAD_STATE:   do_read_thread_state(user); return;
		case WRITE_THREAD_STATE:  do_write_thread_state(user); return;
		case REQUEST_AND_WAIT:    do_request_and_wait(user); return;
		case REPLY:               do_reply(user); return;
		case WAIT_FOR_REQUEST:    do_wait_for_request(user); return;
		case SET_PAGER:           do_set_pager(user); return;
		case UPDATE_PD:           do_update_pd(user); return;
		case UPDATE_REGION:       do_update_region(user); return;
		case NEW_PD:              do_new_pd(user); return;
		case ALLOCATE_IRQ:        do_allocate_irq(user); return;
		case AWAIT_IRQ:           do_await_irq(user); return;
		case FREE_IRQ:            do_free_irq(user); return;
		case PRINT_CHAR:          do_print_char(user); return;
		case NEW_SIGNAL_RECEIVER: do_new_signal_receiver(user); return;
		case NEW_SIGNAL_CONTEXT:  do_new_signal_context(user); return;
		case KILL_SIGNAL_CONTEXT: do_kill_signal_context(user); return;
		case AWAIT_SIGNAL:        do_await_signal(user); return;
		case SUBMIT_SIGNAL:       do_submit_signal(user); return;
		case SIGNAL_PENDING:      do_signal_pending(user); return;
		case ACK_SIGNAL:          do_ack_signal(user); return;
		case NEW_VM:              do_new_vm(user); return;
		case RUN_VM:              do_run_vm(user); return;
		case PAUSE_VM:            do_pause_vm(user); return;
		default:
			PERR("invalid syscall");
			user->crash();
			reset_lap_time();
		}
	}
}

/**
 * Prepare the system for the first run of 'kernel'
 */
extern "C" void init_phys_kernel() {
	Cpu::init_phys_kernel(); }

/**
 * Kernel main routine
 */
extern "C" void kernel()
{
	static bool initial_call = true;

	/* an exception occured */
	if (!initial_call)
	{
		/* handle exception that interrupted the last user */
		cpu_scheduler()->head()->handle_exception();

	/* kernel initialization */
	} else {

		Genode::printf("Kernel started!\n");

		/* enable kernel timer */
		pic()->unmask(Timer::IRQ);

		/* compose kernel CPU context */
		static Cpu::Context kernel_context;
		kernel_context.ip = (addr_t)kernel;
		kernel_context.sp = (addr_t)&_kernel_stack_high;

		/* add kernel to the core PD */
		core()->append_context(&kernel_context);

		/* offer the final kernel context to the mode transition page */
		mtc()->fetch_master_context(&kernel_context);

		/* TrustZone initialization code */
		trustzone_initialization(pic());

		/* switch to core address space */
		Cpu::init_virt_kernel(core()->tlb()->base(), core_id());

		/*
		 * From this point on, it is safe to use 'cmpxchg', i.e., to create
		 * singleton objects via the static-local object pattern. See
		 * the comment in 'src/base/singleton.h'.
		 */

		/* create the core main thread */
		static Native_utcb cm_utcb;
		static char cm_stack[DEFAULT_STACK_SIZE]
		            __attribute__((aligned(Cpu::DATA_ACCESS_ALIGNM)));
		static Thread core_main((Platform_thread *)0);
		_main_utcb = &cm_utcb;
		enum { CM_STACK_SIZE = sizeof(cm_stack)/sizeof(cm_stack[0]) + 1 };
		core_main.start((void *)CORE_MAIN,
		                (void *)&cm_stack[CM_STACK_SIZE - 1],
		                0, core_id(), &cm_utcb, &cm_utcb);

		/* kernel initialization finished */
		reset_lap_time();
		initial_call = false;
	}
	/* will jump to the context related mode-switch */
	cpu_scheduler()->head()->proceed();
}


/********************
 ** Kernel::Thread **
 ********************/

void Kernel::Thread::crash()
{
	cpu_scheduler()->remove(this);
	_state = CRASHED;
}

int Kernel::Thread::resume()
{
	switch (_state) {
	case AWAIT_RESUMPTION:
		_schedule();
		return 0;
	case SCHEDULED:
		return 1;
	case AWAIT_IPC:
		PDBG("cancel IPC receipt");
		Ipc_node::cancel_waiting();
		_schedule();
		return 0;
	case AWAIT_IRQ:
		PDBG("cancel IRQ receipt");
		Irq_receiver::cancel_waiting();
		_schedule();
		return 0;
	case AWAIT_SIGNAL:
		PDBG("cancel signal receipt");
		_signal_receiver->remove_listener(this);
		_schedule();
		return 0;
	case AWAIT_SIGNAL_CONTEXT_DESTRUCT:
		PDBG("cancel signal context destruction");
		_schedule();
		return 0;
	case AWAIT_START:
	default:
		PERR("unresumable state");
		return -1;
	}
}


void Thread::prepare_to_start(void * const        ip,
                              void * const        sp,
                              unsigned const      cpu_id,
                              unsigned const      pd_id,
                              Native_utcb * const utcb_phys,
                              Native_utcb * const utcb_virt)
{
	/* check state and arguments */
	assert(_state == AWAIT_START)
	assert(!cpu_id);

	/* store thread parameters */
	_phys_utcb = utcb_phys;
	_virt_utcb = utcb_virt;
	_pd_id     = pd_id;

	/* join a protection domain */
	Pd * const pd = Pd::pool()->object(_pd_id);
	assert(pd)
	addr_t const tlb = pd->tlb()->base();

	/* initialize CPU context */
	if (!_platform_thread)
		/* this is the main thread of core */
		User_context::init_core_main_thread(ip, sp, tlb, pd_id);
	else if (!_platform_thread->main_thread())
		/* this is not a main thread */
		User_context::init_thread(ip, sp, tlb, pd_id);
	else
		/* this is the main thread of a program other than core */
		User_context::init_main_thread(ip, _virt_utcb, tlb, pd_id);
}


void Thread::start(void * const        ip,
                   void * const        sp,
                   unsigned const      cpu_id,
                   unsigned const      pd_id,
                   Native_utcb * const utcb_phys,
                   Native_utcb * const utcb_virt)
{
	prepare_to_start(ip, sp, cpu_id, pd_id, utcb_phys, utcb_virt);
	_schedule();
}


void Thread::pagefault(addr_t const va, bool const w)
{
	/* pause faulter */
	cpu_scheduler()->remove(this);
	_state = AWAIT_RESUMPTION;

	/* inform pager through IPC */
	assert(_pager);
	_pagefault = Pagefault(id(), (Tlb *)tlb(), ip, va, w);
	Ipc_node::send_note(_pager, &_pagefault, sizeof(_pagefault));
}


void Thread::kill_signal_context_blocks()
{
	cpu_scheduler()->remove(this);
	_state = AWAIT_SIGNAL_CONTEXT_DESTRUCT;
}


void Thread::kill_signal_context_done()
{
	if (_state != AWAIT_SIGNAL_CONTEXT_DESTRUCT) {
		PDBG("ignore unexpected signal-context destruction");
		return;
	}
	user_arg_0(1);
	_schedule();
}


/****************************
 ** Kernel::Signal_context **
 ****************************/

void Signal_context::_deliver()
{
	if (!_submits) return;
	_receiver->deliver(this);
}


void Signal_context::ack()
{
	assert(_await_ack);
	_await_ack = 0;
	if (!_killer) {
		_deliver();
		return;
	}
	_killer->kill_signal_context_done();
	this->~Signal_context();
}


void Signal_context::submit(unsigned const n)
{
	assert(_submits < (unsigned)~0 - n);
	if (_killer) return;
	_submits += n;
	if (_await_ack) return;
	_deliver();
}

