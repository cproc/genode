/*
 * \brief  Genode backend for GDB server (x86_64-specific code)
 * \author Christian Prochaska
 * \date   2014-01-30
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef AMD64_H
#define AMD64_H

/* indices from 'regs_amd64' array in amd64.c */
enum reg_index {
	RAX      =  0,
	RBX      =  1,
	RCX      =  2,
	RDX      =  3,
	RSI      =  4,
	RDI      =  5,
	RBP      =  6,
	RSP      =  7,
	R8       =  8,
	R9       =  9,
	R10      = 10,
	R11      = 11,
	R12      = 12,
	R13      = 13,
	R14      = 14,
	R15      = 15,
	RIP      = 16,
	EFLAGS   = 17,
	CS       = 18,
	SS       = 19,
	DS       = 20,
	ES       = 21,
	FS       = 22,
	GS       = 23,
};

#endif /* AMD64_H */
