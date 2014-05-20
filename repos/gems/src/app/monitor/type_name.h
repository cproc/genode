/*
 * \brief  String type used for resource-type names
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _TYPE_NAME_H_
#define _TYPE_NAME_H_

#include <types.h>

namespace Monitor { typedef Genode::String<64> Type_name; }

#endif /* _TYPE_NAME_H_ */
