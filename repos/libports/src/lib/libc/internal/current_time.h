/*
 * \brief  Interface for obtaining the current time
 * \author Norman Feske
 * \date   2019-09-18
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _LIBC__INTERNAL__CURRENT_TIME_H_
#define _LIBC__INTERNAL__CURRENT_TIME_H_

/* Genode includes */
#include <base/duration.h>

/* libc-internal includes */
#include <internal/types.h>

namespace Libc {

	struct Current_time : Interface
	{
		virtual Duration current_time() = 0;
	};

	struct Current_real_time : Interface
	{
		virtual bool has_real_time() const = 0;

		virtual timespec current_real_time() = 0;
	};
}

#endif /* _LIBC__INTERNAL__CURRENT_TIME_H_ */
