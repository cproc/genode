/*
 * \brief  Fiasco.OC(x86_64)-specific helper functions for GDB server
 * \author Christian Prochaska
 * \date   2014-05-21
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

extern "C" {
#define private _private
#include "genode-low.h"
#define _private private
}

#include <base/printf.h>
#include "amd64.h"
#include "cpu_session_component.h"
#include "gdbserver_platform_helper.h"
#include "gdb_stub_thread.h"

#undef PDBG
#define PDBG(...)

using namespace Genode;

static bool in_syscall(Thread_state const &thread_state)
{
	/* looking for syscall pattern:
	 * EIP-7:  55     push %ebp
	 * EIP-6:  ff 93  call ...
	 * EIP:    5d     pop %ebp
	 */
	if ((genode_read_memory_byte((void*)(thread_state.ip)) == 0x5d) &&
		(genode_read_memory_byte((void*)(thread_state.ip - 5)) == 0x93) &&
		(genode_read_memory_byte((void*)(thread_state.ip - 6)) == 0xff) &&
		(genode_read_memory_byte((void*)(thread_state.ip - 7)) == 0x55))
		return true;

	return false;
}

extern "C" int genode_fetch_register(int regno, unsigned long *reg_content)
{
	Thread_state thread_state;

	try { thread_state = get_current_thread_state(); }
	catch (...) { return 0; }

	if (in_syscall(thread_state) || thread_state.unresolved_page_fault) {
		switch((enum reg_index)regno)
		{
			case RAX:  PDBG("cannot determine contents of register RAX"); return -1;
			case RBX:  PDBG("cannot determine contents of register RBX"); return -1;
			case RCX:  PDBG("cannot determine contents of register RCX"); return -1;
			case RDX:  PDBG("cannot determine contents of register RDX"); return -1;
			case RSI:  PDBG("cannot determine contents of register RSI"); return -1;
			case RDI:  PDBG("cannot determine contents of register RDI"); return -1;
#if 0
			case EBX:
				if (in_syscall(thread_state)) {
					/* When in a syscall, the user EBX has been pushed onto the stack at address ESP+4 */
					*reg_content = genode_read_memory_byte((void*)(thread_state.sp + 4)) +
				               	   (genode_read_memory_byte((void*)(thread_state.sp + 5)) << 8) +
				               	   (genode_read_memory_byte((void*)(thread_state.sp + 6)) << 16) +
				               	   (genode_read_memory_byte((void*)(thread_state.sp + 7)) << 24);
					PDBG("EBX = %16lx", *reg_content);
					return 0;
				} else {
					PDBG("cannot determine contents of register EBX"); return -1;
				}
#endif
			case RBP:
#if 0
				if (in_syscall(thread_state)) {
					/* When in a syscall, the user EBP has been pushed onto the stack at address ESP+0 */
					*reg_content = genode_read_memory_byte((void*)(thread_state.sp + 0)) +
							   	   (genode_read_memory_byte((void*)(thread_state.sp + 1)) << 8) +
							   	   (genode_read_memory_byte((void*)(thread_state.sp + 2)) << 16) +
							   	   (genode_read_memory_byte((void*)(thread_state.sp + 3)) << 24);
					PDBG("EBP = %16lx", *reg_content);
					return 0;
				} else {
#endif
					PDBG("cannot determine contents of register RBP"); return -1;
#if 0
				}
#endif
			case RSP: *reg_content = thread_state.sp; PDBG("RSP = %16lx", *reg_content); return 0;
			case R8:   PDBG("cannot determine contents of register R8"); return -1;
			case R9:   PDBG("cannot determine contents of register R9"); return -1;
			case R10:  PDBG("cannot determine contents of register R10"); return -1;
			case R11:  PDBG("cannot determine contents of register R11"); return -1;
			case R12:  PDBG("cannot determine contents of register R12"); return -1;
			case R13:  PDBG("cannot determine contents of register R13"); return -1;
			case R14:  PDBG("cannot determine contents of register R14"); return -1;
			case R15:  PDBG("cannot determine contents of register R15"); return -1;
			case RIP:  *reg_content = thread_state.ip; PDBG("RIP = %16lx", *reg_content); return 0;
			case EFLAGS:  PDBG("cannot determine contents of register EFLAGS"); return -1;
			case CS:   PDBG("cannot determine contents of register CS"); return -1;
			case SS:   PDBG("cannot determine contents of register SS"); return -1;
			case DS:   PDBG("cannot determine contents of register DS"); return -1;
			case ES:   PDBG("cannot determine contents of register ES"); return -1;
			case FS:   PDBG("cannot determine contents of register FS"); return -1;
			case GS:   PDBG("cannot determine contents of register GS"); return -1;
		}
	} else {
		switch((enum reg_index)regno)
		{
			case RAX:     *reg_content = thread_state.rax; PDBG("RAX = %16lx", *reg_content); return 0;
			case RBX:     *reg_content = thread_state.rbx; PDBG("RBX = %16lx", *reg_content); return 0;
			case RCX:     *reg_content = thread_state.rcx; PDBG("RCX = %16lx", *reg_content); return 0;
			case RDX:     *reg_content = thread_state.rdx; PDBG("RDX = %16lx", *reg_content); return 0;
			case RSI:     *reg_content = thread_state.rsi; PDBG("RSI = %16lx", *reg_content); return 0;
			case RDI:     *reg_content = thread_state.rdi; PDBG("RDI = %16lx", *reg_content); return 0;
			case RBP:     *reg_content = thread_state.rbp; PDBG("RBP = %16lx", *reg_content); return 0;
			case RSP:     *reg_content = thread_state.sp; PDBG("RSP = %16lx", *reg_content); return 0;
			case R8:      *reg_content = thread_state.r8; PDBG("R8 = %16lx", *reg_content); return 0;
			case R9:      *reg_content = thread_state.r9; PDBG("R9 = %16lx", *reg_content); return 0;
			case R10:     *reg_content = thread_state.r10; PDBG("R10 = %16lx", *reg_content); return 0;
			case R11:     *reg_content = thread_state.r11; PDBG("R11 = %16lx", *reg_content); return 0;
			case R12:     *reg_content = thread_state.r12; PDBG("R12 = %16lx", *reg_content); return 0;
			case R13:     *reg_content = thread_state.r13; PDBG("R13 = %16lx", *reg_content); return 0;
			case R14:     *reg_content = thread_state.r14; PDBG("R14 = %16lx", *reg_content); return 0;
			case R15:     *reg_content = thread_state.r15; PDBG("R15 = %16lx", *reg_content); return 0;
			case RIP:     *reg_content = thread_state.ip; PDBG("RIP = %16lx", *reg_content); return 0;
			case EFLAGS:  *reg_content = thread_state.eflags; PDBG("EFLAGS = %16lx", *reg_content); return 0;
			case CS:      PDBG("cannot determine contents of register CS"); return -1;
			case SS:      PDBG("cannot determine contents of register SS"); return -1;
			case DS:      PDBG("cannot determine contents of register DS"); return -1;
			case ES:      PDBG("cannot determine contents of register ES"); return -1;
			case FS:      PDBG("cannot determine contents of register FS"); return -1;
			case GS:      PDBG("cannot determine contents of register GS"); return -1;
		}
	}

	return -1;
}

extern "C" void genode_store_register(int regno, unsigned long reg_content)
{
	PDBG("not implemented yet for this platform");
#if 0
	Thread_state thread_state;

	try { thread_state = get_current_thread_state(); }
	catch (...) { return; }

	if (in_syscall(thread_state)) {
		PDBG("cannot set registers while thread is in syscall");
		return;
	}

	switch((enum reg_index)regno)
	{
		case EAX:  thread_state.eax    = reg_content; PDBG("EAX = %16lx", reg_content); break;
		case ECX:  thread_state.ecx    = reg_content; PDBG("ECX = %16lx", reg_content); break;
		case EDX:  thread_state.edx    = reg_content; PDBG("EDX = %16lx", reg_content); break;
		case EBX:  thread_state.ebx    = reg_content; PDBG("EBX = %16lx", reg_content); break;
		case UESP: thread_state.sp     = reg_content; PDBG("ESP = %16lx", reg_content); break;
		case EBP:  thread_state.ebp    = reg_content; PDBG("EBP = %16lx", reg_content); break;
		case ESI:  thread_state.esi    = reg_content; PDBG("ESI = %16lx", reg_content); break;
		case EDI:  thread_state.edi    = reg_content; PDBG("EDI = %16lx", reg_content); break;
		case EIP:  thread_state.ip     = reg_content; PDBG("EIP = %16lx", reg_content); break;
		case EFL:  thread_state.eflags = reg_content; PDBG("EFL = %16lx", reg_content); break;
		case CS:   PDBG("cannot set contents of register CS"); PDBG(" CS = %16lx", reg_content); break;
		case SS:   PDBG("cannot set contents of register SS"); PDBG(" SS = %16lx", reg_content); break;
		case DS:   PDBG("cannot set contents of register DS"); PDBG(" DS = %16lx", reg_content); break;
		case ES:   PDBG("cannot set contents of register ES"); PDBG(" ES = %16lx", reg_content); break;
		case FS:   thread_state.fs     = reg_content; PDBG(" FS = %16lx", reg_content); break;
		case GS:   thread_state.gs     = reg_content; PDBG(" GS = %16lx", reg_content); break;

	}

	set_current_thread_state(thread_state);
#endif
}

