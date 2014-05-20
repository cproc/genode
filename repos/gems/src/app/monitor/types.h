/*
 * \brief  Common type definitions for the 'Monitor' namespace
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _TYPES_H_
#define _TYPES_H_

/* Genode includes */
#include <util/xml_node.h>
#include <util/xml_generator.h>
#include <base/allocator.h>
#include <util/volatile_object.h>
#include <os/server.h>

/* local includes */
#include <xml_utils.h>

namespace Monitor {
	using Genode::Allocator;
	using Genode::string_attribute;
	using Genode::Xml_node;
	using Genode::Xml_generator;
	using Genode::Lazy_volatile_object;
	using Genode::Signal_context_capability;
	using Genode::Signal_rpc_member;
	using Genode::Noncopyable;
	using Server::Entrypoint;
	using Genode::size_t;
}

#endif /* _TYPES_H_ */
