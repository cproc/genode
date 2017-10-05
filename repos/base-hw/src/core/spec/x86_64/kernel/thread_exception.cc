/*
 * \brief  Kernel backend for execution contexts in userland
 * \author Adrian-Ken Rueegsegger
 * \author Reto Buerki
 * \author Stefan Kalkowski
 * \date   2015-02-09
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* core includes */
#include <kernel/cpu.h>
#include <kernel/thread.h>

using namespace Kernel;

void Thread::exception(unsigned const cpu)
{
	using Genode::Cpu_state;

	switch (regs->trapno) {
	case Cpu_state::PAGE_FAULT:
		_mmu_exception();
		return;
	case Cpu_state::NO_MATH_COPROC:
		if (_cpu->fpu().fault(fpu_regs)) { return; }
		Genode::warning(*this, ": FPU error");
		_die();
		return;
	case Cpu_state::UNDEFINED_INSTRUCTION:
		Genode::warning(*this, ": undefined instruction at ip=", (void*)regs->ip);
		_die();
		return;
	case Cpu_state::SUPERVISOR_CALL:
		_call();
		return;
	}
	if (regs->trapno >= Cpu_state::INTERRUPTS_START &&
	    regs->trapno <= Cpu_state::INTERRUPTS_END) {
		_interrupt(cpu);
		return;
	}
	Genode::warning(*this, ": triggered unknown exception ", regs->trapno,
	                " with error code ", regs->errcode, " at ip=", (void*)regs->ip);
	_die();
}
