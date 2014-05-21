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
#include "gdb_stub_thread.h"

using namespace Genode;

extern "C" int genode_fetch_register(int regno, unsigned long *reg_content)
{
	Thread_state thread_state;
PDBG("called");
	try { thread_state = get_current_thread_state(); }
	catch (...) { PDBG("could not get thread state"); return -1; }

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
	PDBG("not implemented yet for this platform");
}

