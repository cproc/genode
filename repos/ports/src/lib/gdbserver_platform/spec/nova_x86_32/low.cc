/*
 * \brief  NOVA (x86_32) specific helper functions for GDB server
 * \author Alexander Boettcher
 * \date   2012-08-09
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include "i386.h"
#include "cpu_session_component.h"
#include "gdbserver_platform_helper.h"
#include "genode_child_resources.h"

using namespace Genode;

//#undef PDBG
//#define PDBG(...)

extern "C" int genode_fetch_register(int regno, unsigned long *reg_content)
{
	Thread_state thread_state;

	try { thread_state = get_current_thread_state(); }
	catch (...) {
		PERR("%s: could not get current thread state (%p)", __PRETTY_FUNCTION__, __builtin_return_address(0));
		return -1;
	}

	switch((enum reg_index)regno)
	{
		case EAX:  *reg_content = thread_state.eax;    PDBG("EAX = %8lx", *reg_content); return 0;
		case ECX:  *reg_content = thread_state.ecx;    PDBG("ECX = %8lx", *reg_content); return 0;
		case EDX:  *reg_content = thread_state.edx;    PDBG("EDX = %8lx", *reg_content); return 0;
		case EBX:  *reg_content = thread_state.ebx;    PDBG("EBX = %8lx", *reg_content); return 0;
		case UESP: *reg_content = thread_state.sp;     PDBG("ESP = %8lx", *reg_content); return 0;
		case EBP:  *reg_content = thread_state.ebp;    PDBG("EBP = %8lx", *reg_content); return 0;
		case ESI:  *reg_content = thread_state.esi;    PDBG("ESI = %8lx", *reg_content); return 0;
		case EDI:  *reg_content = thread_state.edi;    PDBG("EDI = %8lx", *reg_content); return 0;
		case EIP:  *reg_content = thread_state.ip;     PDBG("EIP = %8lx", *reg_content); return 0;
		case EFL:  *reg_content = thread_state.eflags; PDBG("EFL = %8lx", *reg_content); return 0;
		case CS:   return -1;
		case SS:   return -1;
		case DS:   return -1;
		case ES:   return -1;
		case FS:   return -1;
		case GS:   return -1;
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

	switch ((enum reg_index)regno)
	{
		case EAX:  thread_state.eax = reg_content; PDBG("EAX = %8lx", reg_content); break;
		case ECX:  thread_state.ecx = reg_content; PDBG("ECX = %8lx", reg_content); break;
		case EDX:  thread_state.edx = reg_content; PDBG("EDX = %8lx", reg_content); break;
		case EBX:  thread_state.ebx = reg_content; PDBG("EBX = %8lx", reg_content); break;
		case UESP: thread_state.sp = reg_content; PDBG("ESP = %8lx", reg_content); break;
		case EBP:  thread_state.ebp = reg_content; PDBG("EBP = %8lx", reg_content); break;
		case ESI:  thread_state.esi = reg_content; PDBG("ESI = %8lx", reg_content); break;
		case EDI:  thread_state.edi = reg_content; PDBG("EDI = %8lx", reg_content); break;
		case EIP:  thread_state.ip = reg_content; PDBG("EIP = %8lx", reg_content); break;
		case EFL:  thread_state.eflags = reg_content; PDBG("EFL = %8lx", reg_content); break;
		case CS:   PDBG("cannot set contents of register CS"); PDBG(" CS = %8lx", reg_content); break;
		case SS:   PDBG("cannot set contents of register SS"); PDBG(" SS = %8lx", reg_content); break;
		case DS:   PDBG("cannot set contents of register DS"); PDBG(" DS = %8lx", reg_content); break;
		case ES:   PDBG("cannot set contents of register ES"); PDBG(" ES = %8lx", reg_content); break;
		case FS:   PDBG("cannot set contents of register FS"); PDBG(" FS = %8lx", reg_content); break;
		case GS:   PDBG("cannot set contents of register GS"); PDBG(" GS = %8lx", reg_content); break;
	}

	set_current_thread_state(thread_state);
}

