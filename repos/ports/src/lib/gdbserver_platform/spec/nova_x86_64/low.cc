/*
 * \brief  NOVA (x86_64) specific helper functions for GDB server
 * \author Alexander Boettcher
 * \author Christian Prochaska
 * \date   2014-01-30
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include "amd64.h"
#include "cpu_session_component.h"
#include "gdbserver_platform_helper.h"
#include "genode_child_resources.h"

using namespace Genode;

extern "C" int genode_fetch_register(int regno, unsigned long *reg_content)
{
	Thread_state thread_state;

	try { thread_state = get_current_thread_state(); }
	catch (...) { return -1; }

	switch((enum reg_index)regno)
	{
		case RAX:     *reg_content = thread_state.rax;    return 0;
		case RBX:     *reg_content = thread_state.rbx;    return 0;
		case RCX:     *reg_content = thread_state.rcx;    return 0;
		case RDX:     *reg_content = thread_state.rdx;    return 0;
		case RSI:     *reg_content = thread_state.rsi;    return 0;
		case RDI:     *reg_content = thread_state.rdi;    return 0;
		case RBP:     *reg_content = thread_state.rbp;    return 0;
		case RSP:     *reg_content = thread_state.sp;     return 0;
		case R8:      *reg_content = thread_state.r8;     return 0;
		case R9:      *reg_content = thread_state.r9;     return 0;
		case R10:     *reg_content = thread_state.r10;    return 0;
		case R11:     *reg_content = thread_state.r11;    return 0;
		case R12:     *reg_content = thread_state.r12;    return 0;
		case R13:     *reg_content = thread_state.r13;    return 0;
		case R14:     *reg_content = thread_state.r14;    return 0;
		case R15:     *reg_content = thread_state.r15;    return 0;
		case RIP:     *reg_content = thread_state.ip;     return 0;
		case EFLAGS:  *reg_content = thread_state.eflags; return 0;
		case CS:      return -1;
		case SS:      return -1;
		case DS:      return -1;
		case ES:      return -1;
		case FS:      return -1;
		case GS:      return -1;
	}

	return -1;
}


extern "C" void genode_store_register(int regno, unsigned long reg_content)
{
	Thread_state thread_state;

	try { thread_state = get_current_thread_state(); }
	catch (...) {
		PERR("%s: could not get current thread state", __PRETTY_FUNCTION__);
		return;
	}

	switch ((enum reg_index)regno) {
		case RAX:     thread_state.rax    = reg_content; PDBG(" RAX = %8lx", reg_content); break;
		case RBX:     thread_state.rbx    = reg_content; PDBG(" RBX = %8lx", reg_content); break;
		case RCX:     thread_state.rcx    = reg_content; PDBG(" RCX = %8lx", reg_content); break;
		case RDX:     thread_state.rdx    = reg_content; PDBG(" RDX = %8lx", reg_content); break;
		case RSI:     thread_state.rsi    = reg_content; PDBG(" RSI = %8lx", reg_content); break;
		case RDI:     thread_state.rdi    = reg_content; PDBG(" RDI = %8lx", reg_content); break;
		case RBP:     thread_state.rbp    = reg_content; PDBG(" RBP = %8lx", reg_content); break;
		case RSP:     thread_state.sp     = reg_content; PDBG(" RSP = %8lx", reg_content); break;
		case R8:      thread_state.r8     = reg_content; PDBG(" R8 = %8lx", reg_content); break;
		case R9:      thread_state.r9     = reg_content; PDBG(" R9 = %8lx", reg_content); break;
		case R10:     thread_state.r10    = reg_content; PDBG(" R10 = %8lx", reg_content); break;
		case R11:     thread_state.r11    = reg_content; PDBG(" R11 = %8lx", reg_content); break;
		case R12:     thread_state.r12    = reg_content; PDBG(" R12 = %8lx", reg_content); break;
		case R13:     thread_state.r13    = reg_content; PDBG(" R13 = %8lx", reg_content); break;
		case R14:     thread_state.r14    = reg_content; PDBG(" R14 = %8lx", reg_content); break;
		case R15:     thread_state.r15    = reg_content; PDBG(" R15 = %8lx", reg_content); break;
		case RIP:     thread_state.ip     = reg_content; PDBG(" RIP = %8lx", reg_content); break;
		case EFLAGS:  thread_state.eflags = reg_content; PDBG(" RFLAGS = %8lx", reg_content); break;
		case CS:      PDBG("cannot set contents of register CS"); PDBG(" CS = %8lx", reg_content); break;
		case SS:      PDBG("cannot set contents of register SS"); PDBG(" SS = %8lx", reg_content); break;
		case DS:      PDBG("cannot set contents of register DS"); PDBG(" DS = %8lx", reg_content); break;
		case ES:      PDBG("cannot set contents of register ES"); PDBG(" ES = %8lx", reg_content); break;
		case FS:      PDBG("cannot set contents of register FS"); PDBG(" FS = %8lx", reg_content); break;
		case GS:      PDBG("cannot set contents of register GS"); PDBG(" GS = %8lx", reg_content); break;
	}

	set_current_thread_state(thread_state);
}

