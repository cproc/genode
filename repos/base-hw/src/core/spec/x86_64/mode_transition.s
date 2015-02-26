/*
 * \brief  Transition between kernel/userland, and secure/non-secure world
 * \author Martin Stein
 * \author Stefan Kalkowski
 * \date   2011-11-15
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

.include "macros.s"

/* size of pointer to CPU context */
.set CONTEXT_PTR_SIZE, 1 * 8

/* globally mapped buffer storage */
.set BUFFER_SIZE, 6 * 8

/* offsets of the member variables in a CPU context */
.set SP_OFFSET,       1 * 8
.set R8_OFFSET,       2 * 8
.set RAX_OFFSET,     10 * 8
.set ERRCODE_OFFSET, 17 * 8
.set FLAGS_OFFSET,   18 * 8
.set TRAPNO_OFFSET,  19 * 8
.set CR3_OFFSET,     21 * 8

.macro _isr_entry
	.align 4, 0x90
.endm

.macro _exception vector
	_isr_entry
	push $0
	push $\vector
	jmp _mt_kernel_entry_pic
.endm

.macro _exception_with_code vector
	_isr_entry
	nop
	nop
	push $\vector
	jmp _mt_kernel_entry_pic
.endm

.section .text

	/*
	 * Page aligned base of mode transition code.
	 *
	 * This position independent code switches between a kernel context and a
	 * user context and thereby between their address spaces. Due to the latter
	 * it must be mapped executable to the same region in every address space.
	 * To enable such switching, the kernel context must be stored within this
	 * region, thus one should map it solely accessable for privileged modes.
	 */
	.p2align MIN_PAGE_SIZE_LOG2
	.global _mt_begin
	_mt_begin:

	/*
	 * On user exceptions the CPU has to jump to one of the following
	 * Interrupt Service Routines (ISRs) to switch to a kernel context.
	 */
	.global _mt_isrs
	_mt_isrs:
	_exception              0
	_exception              1
	_exception              2
	_exception              3
	_exception              4
	_exception              5
	_exception              6
	_exception              7
	_exception_with_code    8
	_exception              9
	_exception_with_code    10
	_exception_with_code    11
	_exception_with_code    12
	_exception_with_code    13
	_exception_with_code    14
	_exception              15
	_exception              16
	_exception_with_code    17
	_exception              18
	_exception              19

	.set vec, 20
	.rept 236
	_exception              vec
	.set vec, vec + 1
	.endr

	/* space for a copy of the kernel context */
	.p2align 2
	.global _mt_master_context_begin
	_mt_master_context_begin:

	/* space must be at least as large as 'Cpu_state' */
	.space 22*8

	.global _mt_master_context_end
	_mt_master_context_end:

	/* space for a client context-pointer per CPU */
	.p2align 2
	.global _mt_client_context_ptr
	_mt_client_context_ptr:
	.space CONTEXT_PTR_SIZE

	/* a globally mapped buffer per CPU */
	.p2align 2
	.global _mt_buffer
	_mt_buffer:
	.space BUFFER_SIZE

	.global _mt_kernel_entry_pic
	_mt_kernel_entry_pic:

	/* Copy client context RAX to buffer */
	mov %rax, _mt_buffer

	/* Switch to kernel page tables */
	mov _mt_master_context_begin+CR3_OFFSET, %rax
	mov %rax, %cr3

	/* Save information on interrupt stack frame in client context */
	mov _mt_client_context_ptr, %rax
	popq TRAPNO_OFFSET(%rax)
	popq ERRCODE_OFFSET(%rax)
	popq (%rax)
	popq FLAGS_OFFSET(%rax) /* Discard cs */
	popq FLAGS_OFFSET(%rax)
	popq SP_OFFSET(%rax)

	/* Save register values to client context */
	lea ERRCODE_OFFSET(%rax), %rsp
	pushq %rbp
	pushq %rsi
	pushq %rdi
	pushq %rdx
	pushq %rcx
	pushq %rbx
	pushq _mt_buffer
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %r12
	pushq %r11
	pushq %r10
	pushq %r9
	pushq %r8

	/* Restore kernel segment registers */
	mov $0x10, %rbx
	mov %rbx, %ds
	mov %rbx, %es
	mov %rbx, %fs
	mov %rbx, %gs

	/* Restore register values from kernel context */
	mov $_mt_master_context_begin+R8_OFFSET, %rsp
	popq %r8
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
	popq %rax
	popq %rbx
	popq %rcx
	popq %rdx
	popq %rdi
	popq %rsi
	popq %rbp

	/* Restore kernel stack and continue kernel execution */
	mov _mt_master_context_begin+SP_OFFSET, %rsp
	jmp *_mt_master_context_begin

	.global _mt_user_entry_pic
	_mt_user_entry_pic:

	/* Prepare stack frame in mt buffer (Intel SDM Vol. 3A, figure 6-8) */
	mov _mt_client_context_ptr, %rax
	mov $_mt_buffer+BUFFER_SIZE, %rsp
	pushq $0x23
	pushq SP_OFFSET(%rax)

	/* Set I/O privilege level to 3 */
	orq $0x3000, FLAGS_OFFSET(%rax)
	btrq $9, FLAGS_OFFSET(%rax) /* XXX: Drop once interrupt handling is done */
	pushq FLAGS_OFFSET(%rax)

	pushq $0x1b
	pushq (%rax)

	/* Restore segment registers */
	mov $0x23, %rbx
	mov %rbx, %ds
	mov %rbx, %es
	mov %rbx, %fs
	mov %rbx, %gs

	/* Restore register values from client context */
	lea R8_OFFSET(%rax), %rsp
	popq %r8
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
	popq _mt_buffer
	popq %rbx
	popq %rcx
	popq %rdx
	popq %rdi
	popq %rsi
	popq %rbp

	/* Switch page tables */
	mov CR3_OFFSET(%rax), %rax
	mov %rax, %cr3

	/* Set stack back to mt buffer and restore client RAX */
	mov $_mt_buffer, %rsp
	popq %rax

	iretq

	/* end of the mode transition code */
	.global _mt_end
	_mt_end:
	1: jmp 1b
