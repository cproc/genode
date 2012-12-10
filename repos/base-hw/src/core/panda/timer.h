/*
 * \brief  Timer for kernel
 * \author Martin Stein
 * \date   2012-04-23
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _PANDA__TIMER_H_
#define _PANDA__TIMER_H_

/* core includes */
#include <timer/cortex_a9.h>

namespace Kernel
{
	/**
	 * Kernel timer
	 */
	class Timer : public Cortex_a9::Timer { };
}

#endif /* _PANDA__TIMER_H_ */

