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

/* Genode includes */
#include <base/allocator.h>

/* local includes */
#include <types.h>
#include <type.h>
#include <type_registry.h>
#include <type_arg.h>
#include <resource.h>
#include <instance.h>

using namespace Monitor;


Type::Type(Allocator &alloc, Key const &key)
:
	Registry<Type>::Element(alloc, key)
{ }


Instance_base &Type::create_instance(Allocator &alloc,
                                     Instance_base const &parent,
                                     Resource const &resource) const
{
	return *new (alloc) Instance(alloc, parent, resource);
}


void Type::destroy_instance(Allocator &alloc, Instance_base const &instance) const
{
	Genode::destroy(&alloc, const_cast<Instance_base *>(&instance));
}


Type_arg const &Type::lookup_type_arg(Type_arg::Key const &key) const
{
	return _arguments.lookup(key);
}


void Type::update(Allocator &alloc, Xml_node node,
                  Type_registry_base const &outer_types)
{
	try {
		_arguments.update(node,
			[]  (Xml_node) { return true; },
			[&] (Xml_node n) { return new (alloc) Type_arg(alloc, n); },
			[&] (Type_arg &arg) { Genode::destroy(&alloc, &arg); },
			[&] (Type_arg &arg, Xml_node node) { arg.update(alloc, node,
			                                                outer_types); }); }
	catch (...) {
		PERR("invalid definition of type \"%s\"", key().string());
		throw; }

	/*
	 * Update aggregated resources
	 */
	try {
		_resources.update(alloc, node, outer_types, _arguments); }
	catch (...) {
		PERR("invalid resource definition within type definition");
		throw; }
}


void Type::serialize(Xml_generator &xml) const
{
	xml.attribute("name", key().string());

	_arguments.serialize_elements(xml);

	_resources.serialize_elements(xml);
}

