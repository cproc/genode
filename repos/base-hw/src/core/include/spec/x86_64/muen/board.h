/*
 * \brief  x86_64_muen constants
 * \author Adrian-Ken Rueegsegger
 * \date   2015-07-02
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _BOARD_H_
#define _BOARD_H_

namespace Genode
{
	struct Board
	{
		enum {
			SINFO_BASE_ADDR = 0xe00000000,
			SINFO_SIZE      = 0x7000,
			TIMER_BASE_ADDR = 0xe00010000,
			TIMER_SIZE      = 0x1000,

			VECTOR_REMAP_BASE   = 48,
			TIMER_VECTOR_KERNEL = 32,
			TIMER_VECTOR_USER   = 50,
		};
	};
}

#endif /* _BOARD_H_ */
