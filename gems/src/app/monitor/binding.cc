/*
 * \brief  Binding of an argument to a resource
 * \author Norman Feske
 * \date   2014-01-20
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <types.h>
#include <resource.h>
#include <type.h>
#include <binding.h>
#include <type_arg.h>

using namespace Monitor;


Binding::Binding(Allocator &alloc, Xml_node node)
:
	Registry<Binding>::Element(alloc, string_attribute<Keyed::Key>(node, "arg"))
{ }


void Binding::bind(Allocator &alloc, Xml_node node, Type_arg const *type_arg,
                   Registry<Resource> const &resources,
                   Registry<Type_arg> const &arguments)
{
	if (!node.has_attribute("ref")) {
		PERR("missing reference in argument %s", key().string());
		throw Xml_node::Nonexistent_attribute(); }

	Ref_name const ref_name = string_attribute<Ref_name>(node, "ref");

	_type_arg     = type_arg;
	_resource_ptr = 0;
	_type_arg_ptr = 0;

	/*
	 * The 'ref' attribute refers to either a resource name in the current
	 * aggregate scope or an argument name.
	 */
	try {
		_resource_ptr = &resources.lookup(ref_name);
		return; }
	catch (...) { }

	try {
		_type_arg_ptr = &arguments.lookup(ref_name); }
	catch (...) {
		PERR("unresolvable reference \"%s\" of resource argument \"%s\"",
		     ref_name.string(), key().string());
		throw; }
}


Type const *Binding::type_ptr() const
{
	if (_resource_ptr)
		return _resource_ptr->type_ptr();

	if (_type_arg_ptr)
		return _type_arg_ptr->type_ptr();

	return 0;
}


bool Binding::belongs_to(Type_arg const &type_arg) const
{
	return _type_arg == &type_arg;
}


void Binding::serialize(Xml_generator &xml) const
{
	if (type_ptr())
		xml.attribute("deduced_type", type_ptr()->key().string());
	else
		PERR("could not deduce type for binding %s", key().string());

	if (_resource_ptr)
		xml.attribute("ref", _resource_ptr->key().string());
	else if (_type_arg_ptr)
		xml.attribute("ref", _type_arg_ptr->key().string());

	xml.attribute("arg", key().string());
}
