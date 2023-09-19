/*
 * \brief  Virtual Machine Monitor VM definition
 * \author Stefan Kalkowski
 * \author Martin Stein
 * \date   2012-06-25
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _VM_BASE_H_
#define _VM_BASE_H_

/* Genode includes */
#include <dataspace/client.h>
#include <rom_session/connection.h>
#include <vm_session/connection.h>
#include <vm_session/handler.h>
#include <util/noncopyable.h>
#include <cpu/vm_state_trustzone.h>

/* local includes */
#include <ram.h>

namespace Genode
{
	class Board_revision;
	class Vm_base;
	class Machine_type;
}

struct Genode::Board_revision
{
	unsigned long value;

	explicit Board_revision(unsigned long value) : value(value) { }
};


struct Genode::Machine_type
{
	unsigned long value;

	explicit Machine_type(unsigned long value) : value(value) { }
};


class Genode::Vm_base : Noncopyable, Interface
{
	public:

		using Kernel_name  = String<32>;
		using Command_line = String<64>;
		struct State_container { Vm_state &ref; };

	private:

		Vm_base(Vm_base const &);
		Vm_base &operator = (Vm_base const &);

	protected:

		Env                  &_env;
		Kernel_name    const &_kernel;
		Command_line   const &_cmdline;
		off_t          const  _kernel_off;
		Machine_type   const  _machine;
		Board_revision const  _board;
		Ram            const  _ram;

		Vm_connection               _vm          { _env };
		Vm_connection::Exit_config  _exit_config { };
		Vm_connection::Vcpu         _vcpu;

		void _load_kernel();

		virtual void   _load_kernel_surroundings() = 0;
		virtual addr_t _board_info_offset() const  = 0;

	public:

		struct Inject_irq_failed         : Exception { };
		struct Exception_handling_failed : Exception { };

		Vm_base(Env                &env,
		        Kernel_name  const &kernel,
		        Command_line const &cmdline,
		        addr_t              ram_base,
		        size_t              ram_size,
		        off_t               kernel_off,
		        Machine_type        machine,
		        Board_revision      board,
		        Allocator          &alloc,
		        Vcpu_handler_base  &handler);

		void   start();
		void   dump();
		void   inject_irq(unsigned irq);
		addr_t va_to_pa(addr_t va);

		Genode::Constructible<State_container> state_container {};
		Vm_state       &state() const { return state_container->ref; }
		Ram      const &ram()   const { return _ram;   }

		template<typename FN>
		void with_state(FN const & fn)
		{
			_vcpu.with_state(fn);
		}


		addr_t smc_arg_0() { return state().r0; }
		addr_t smc_arg_1() { return state().r1; }
		addr_t smc_arg_2() { return state().r2; }
		addr_t smc_arg_3() { return state().r3; }
		addr_t smc_arg_4() { return state().r4; }
		addr_t smc_arg_5() { return state().r5; }
		addr_t smc_arg_6() { return state().r6; }
		addr_t smc_arg_7() { return state().r7; }
		addr_t smc_arg_8() { return state().r8; }
		addr_t smc_arg_9() { return state().r9; }

		void smc_ret(addr_t const ret_0) { state().r0 = ret_0; }
		void smc_ret(addr_t const ret_0, addr_t const ret_1);
};

#endif /* _VM_BASE_H_ */
