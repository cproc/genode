/*
 * \brief  Representation of type argument
 * \author Norman Feske
 * \date   2014-01-18
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <type_arg.h>
#include <type_registry.h>
#include <type.h>

using namespace Monitor;


void Type_arg::update(Allocator &alloc, Xml_node node,
                      Type_registry_base const &types)
{
	try {
		_type_ptr = 0;
		Type_name const type_name = string_attribute<Type_name>(node, "type");
		_type_ptr = &types.type_by_name(type_name);
	} catch (...) {
		PERR("invalid type for argument \"%s\"", key().string());
		throw;
	}
}


Type_arg::Type_arg(Allocator &alloc, Xml_node node)
:
	Registry<Type_arg>::Element(alloc, string_attribute<Key>(node, "name"))
{ }


Type const *Type_arg::type_ptr() const { return _type_ptr; }


void Type_arg::serialize(Xml_generator &xml) const
{
	if (_type_ptr)
		xml.attribute("type", _type_ptr->key().string());

	xml.attribute("name", key().string());
}
