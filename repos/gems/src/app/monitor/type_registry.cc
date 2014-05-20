/*
 * \brief  Resource-type registry
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <type_registry.h>
#include <type.h>

using namespace Monitor;


void Type_registry::update(Allocator &alloc, Xml_node node)
{
	auto ready_fn = [] (Xml_node) { return true; };

	auto create_fn = [&] (Xml_node node)
	{
		try {
			Type::Key const key = string_attribute<Type::Key>(node, Type::xml_key());
			return new (alloc) Type(alloc, key); }
		catch (Xml_node::Nonexistent_attribute) {
			PERR("unnamed <type>");
			throw; }
	};

	Registry<Type>::update(node, ready_fn, create_fn,
		[&] (Type &type) { Genode::destroy(&alloc, &type); },
		[&] (Type &type, Xml_node node) { type.update(alloc, node, _outer_types); });
}

