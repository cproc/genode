/*
 * \brief  Trace timestamp
 * \author Stefan Kalkowski
 * \date   2019-03-25
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__SPEC__ARM_64__TRACE__TIMESTAMP_H_
#define _INCLUDE__SPEC__ARM_64__TRACE__TIMESTAMP_H_

#include <base/fixed_stdint.h>

namespace Genode { namespace Trace {

	typedef uint32_t Timestamp;

	inline Timestamp timestamp()
	{
		return 0;
	}
} }

#endif /* _INCLUDE__SPEC__ARM_64__TRACE__TIMESTAMP_H_ */
