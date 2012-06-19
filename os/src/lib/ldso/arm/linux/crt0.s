/*
 * \brief   Startup code for Genode applications
 * \author  Christian Helmuth
 * \date    2006-07-06
 */

/*
 * Copyright (C) 2006-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/*--- .text (program code) -------------------------*/
.section ".text.crt0"

	.globl _start_ldso
_start_ldso:

	ldr r1,=__initial_sp
	str sp,[r1]

	/*
	 * environ = &argv[argc + 1]
	 * in Genode argc is always 1
	 */
	add sp,sp,#12
	ldr r1,=lx_environ
	str sp,[r1]

	/* XXX Switch to our own stack.  */
	ldr sp,.stack_high

	/* relocate ldso */
	mov r1,#0
	bl init_rtld

	/* Clear the frame pointer and the link register so that stack
	backtraces will work. */ 
	mov fp,#0
	mov lr,#0

	/* Jump into init C code */
	b _main

	.stack_high: .word _stack_high
