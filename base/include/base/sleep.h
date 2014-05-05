/*
 * \brief  Lay back and relax
 * \author Norman Feske
 * \author Christian Helmuth
 * \date   2006-07-19
 */

/*
 * Copyright (C) 2006-2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__SLEEP_H_
#define _INCLUDE__BASE__SLEEP_H_

#include <base/lock.h>

namespace Genode {

	__attribute__((noreturn)) inline void sleep_forever()
	{
		Lock sleep;
		while (true) sleep.lock();
	}
}

#endif /* _INCLUDE__BASE__SLEEP_H_ */
