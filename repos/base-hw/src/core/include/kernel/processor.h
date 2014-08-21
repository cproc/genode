/*
 * \brief   A multiplexable common instruction processor
 * \author  Martin Stein
 * \author  Stefan Kalkowski
 * \date    2014-01-14
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _KERNEL__PROCESSOR_H_
#define _KERNEL__PROCESSOR_H_

/* core includes */
#include <timer.h>
#include <cpu.h>
#include <kernel/scheduler.h>

/* base includes */
#include <unmanaged_singleton.h>

namespace Kernel
{
	/**
	 * A single user of a multiplexable processor
	 */
	class Processor_client;

	/**
	 * Ability to do a domain update on all processors
	 */
	class Processor_domain_update;

	/**
	 * Execution context that is scheduled on CPU idle
	 */
	class Cpu_idle;

	/**
	 * Multiplexes a single processor to multiple processor clients
	 */
	typedef Scheduler<Processor_client> Processor_scheduler;

	/**
	 * A multiplexable common instruction processor
	 */
	class Processor;

	/**
	 * Provides a processor object for every available processor
	 */
	class Processor_pool;

	/**
	 * Return Processor_pool singleton
	 */
	Processor_pool * processor_pool();
}

class Kernel::Processor_domain_update
:
	public Double_list_item<Processor_domain_update>
{
	friend class Processor_domain_update_list;

	private:

		bool     _pending[PROCESSORS];
		unsigned _domain_id;

		/**
		 * Domain-update back-end
		 */
		void _domain_update() { Cpu::flush_tlb_by_pid(_domain_id); }

		/**
		 * Perform the domain update on the executing processors
		 */
		void _perform_locally();

	protected:

		/**
		 * Constructor
		 */
		Processor_domain_update()
		{
			for (unsigned i = 0; i < PROCESSORS; i++) { _pending[i] = false; }
		}

		/**
		 * Perform the domain update on all processors
		 *
		 * \param domain_id  kernel name of targeted domain
		 *
		 * \return  wether the update blocks and reports back on completion
		 */
		bool _perform(unsigned const domain_id);

		/**
		 * Notice that the update isn't pending on any processor anymore
		 */
		virtual void _processor_domain_update_unblocks() = 0;
};

class Kernel::Processor_client : public Processor_scheduler::Item
{
	protected:

		Processor *    _processor;
		Cpu_lazy_state _lazy_state;

		unsigned _tics_consumed;

		/**
		 * Handle an interrupt exception that occured during execution
		 *
		 * \param processor_id  kernel name of targeted processor
		 */
		void _interrupt(unsigned const processor_id);

		/**
		 * Insert context into the processor scheduling
		 */
		void _schedule();

		/**
		 * Remove context from the processor scheduling
		 */
		void _unschedule();

		/**
		 * Yield currently scheduled processor share of the context
		 */
		void _yield();

	public:

		/**
		 * Handle an exception that occured during execution
		 *
		 * \param processor_id  kernel name of targeted processor
		 */
		virtual void exception(unsigned const processor_id) = 0;

		/**
		 * Continue execution
		 *
		 * \param processor_id  kernel name of targeted processor
		 */
		virtual void proceed(unsigned const processor_id) = 0;

		/**
		 * Constructor
		 *
		 * \param processor  kernel object of targeted processor
		 * \param priority   scheduling priority
		 */
		Processor_client(Processor * const processor, Priority const priority)
		:
			Processor_scheduler::Item(priority),
			_processor(processor),
			_tics_consumed(0)
		{ }

		/**
		 * Destructor
		 */
		~Processor_client()
		{
			if (!_scheduled()) { return; }
			_unschedule();
		}

		/**
		 * Update how many tics the client consumed from its current time slice
		 *
		 * \param tics_left       tics that aren't consumed yet at the slice
		 * \param tics_per_slice  tics that the slice provides
		 */
		void update_tics_consumed(unsigned const tics_left,
		                          unsigned const tics_per_slice)
		{
			unsigned const old_tics_left = tics_per_slice - _tics_consumed;
			_tics_consumed += old_tics_left - tics_left;
		}

		/**
		 * Reset how many tics the client consumed from its current time slice
		 */
		void reset_tics_consumed() { _tics_consumed = 0; }

		/***************
		 ** Accessors **
		 ***************/

		Cpu_lazy_state * lazy_state() { return &_lazy_state; }
		unsigned tics_consumed() { return _tics_consumed; }
};

class Kernel::Cpu_idle : public Cpu::User_context, public Processor_client
{
	private:

		static constexpr size_t stack_size = sizeof(addr_t) * 32;

		char _stack[stack_size] __attribute__((aligned(16)));

		/**
		 * Main function of all idle threads
		 */
		static void _main() { while (1) { Cpu::wait_for_interrupt(); } }

	public:

		/**
		 * Construct idle context for CPU 'cpu'
		 */
		Cpu_idle(Processor * const cpu);

		/**
		 * Handle exception that occured during execution on CPU 'cpu'
		 */
		void exception(unsigned const cpu)
		{
			switch (cpu_exception) {
			case INTERRUPT_REQUEST:      _interrupt(cpu); return;
			case FAST_INTERRUPT_REQUEST: _interrupt(cpu); return;
			case RESET:                                   return;
			default: assert(0); }
		}

		/**
		 * Continue execution on CPU 'cpu_id'
		 */
		void proceed(unsigned const cpu_id);
};

class Kernel::Processor : public Cpu
{
	private:

		unsigned const      _id;
		Cpu_idle            _idle;
		Processor_scheduler _scheduler;
		bool                _ip_interrupt_pending;
		Timer * const       _timer;

		void _start_timer(unsigned const tics) {
			_timer->start_one_shot(tics, _id); }

		unsigned _tics_per_slice() {
			return _timer->ms_to_tics(USER_LAP_TIME_MS); }

		void _update_timer(unsigned const tics_consumed,
		                   unsigned const tics_per_slice)
		{
			assert(tics_consumed <= tics_per_slice);
			if (tics_consumed >= tics_per_slice) { _start_timer(1); }
			else { _start_timer(tics_per_slice - tics_consumed); }
		}

	public:

		/**
		 * Constructor
		 *
		 * \param id     kernel name of the processor
		 * \param timer  scheduling timer
		 */
		Processor(unsigned const id, Timer * const timer)
		:
			_id(id), _idle(this), _scheduler(&_idle),
			_ip_interrupt_pending(false), _timer(timer)
		{ }

		/**
		 * Initializate on the processor that this object corresponds to
		 */
		void init_processor_local() { _update_timer(0, _tics_per_slice()); }

		/**
		 * Check for a scheduling timeout and handle it in case
		 *
		 * \param interrupt_id  kernel name of interrupt that caused this call
		 *
		 * \return  wether it was a timeout and therefore has been handled
		 */
		bool check_timer_interrupt(unsigned const interrupt_id)
		{
			if (_timer->interrupt_id(_id) != interrupt_id) { return false; }
			_scheduler.yield_occupation();
			return true;
		}

		/**
		 * Notice that the inter-processor interrupt isn't pending anymore
		 */
		void ip_interrupt_handled() { _ip_interrupt_pending = false; }

		/**
		 * Raise the inter-processor interrupt of the processor
		 */
		void trigger_ip_interrupt();

		/**
		 * Add a processor client to the scheduling plan of the processor
		 *
		 * \param client  targeted client
		 */
		void schedule(Processor_client * const client);

		/**
		 * Handle exception of the processor and proceed its user execution
		 */
		void exception();

		/***************
		 ** Accessors **
		 ***************/

		unsigned id() const { return _id; }

		Processor_scheduler * scheduler() { return &_scheduler; }
};

class Kernel::Processor_pool
{
	private:

		Timer _timer;
		char  _processors[PROCESSORS][sizeof(Processor)];

	public:

		/**
		 * Constructor
		 */
		Processor_pool()
		{
			for (unsigned id = 0; id < PROCESSORS; id++) {
				new (_processors[id]) Processor(id, &_timer); }
		}

		/**
		 * Return the kernel object of processor 'id'
		 */
		Processor * processor(unsigned const id) const
		{
			assert(id < PROCESSORS);
			char * const p = const_cast<char *>(_processors[id]);
			return reinterpret_cast<Processor *>(p);
		}

		/**
		 * Return the object of the primary processor
		 */
		Processor * primary_processor() const {
			return processor(Processor::primary_id()); }
};

#endif /* _KERNEL__PROCESSOR_H_ */
