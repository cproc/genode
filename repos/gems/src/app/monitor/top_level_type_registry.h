/*
 * \brief  Registry of top-level resource types
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _TOP_LEVEL_TYPE_REGISTRY_H_
#define _TOP_LEVEL_TYPE_REGISTRY_H_

#include <types.h>
#include <type.h>
#include <type_registry.h>

namespace Monitor { class Top_level_type_registry; }


struct Monitor::Top_level_type_registry : Type_registry_base
{
	/**
	 * Exception type
	 */
	class Builtin_type_does_not_exist { };

	/**
	 * Constructor
	 *
	 * \throw Builtin_type_does_not_exist
	 */
	Top_level_type_registry(Allocator &);
};

#endif /* _TOP_LEVEL_TYPE_REGISTRY_H_ */
