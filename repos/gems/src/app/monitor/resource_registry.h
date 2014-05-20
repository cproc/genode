/*
 * \brief  Resource registry
 * \author Norman Feske
 * \date   2014-01-18
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _RESOURCE_REGISTRY_H_
#define _RESOURCE_REGISTRY_H_

/* local includes */
#include <type_registry.h>

namespace Monitor {
	class Resource_registry;
	class Resource;
	class Type_arg;
}


struct Monitor::Resource_registry : Registry<Resource>
{
	void update(Allocator &, Xml_node,
	            Type_registry_base const &, Registry<Type_arg> const &);
};

#endif /* _RESOURCE_REGISTRY_H_ */
