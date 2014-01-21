/*
 * \brief  Representation of resources
 * \author Norman Feske
 * \date   2014-01-18
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <types.h>
#include <resource.h>
#include <type_name.h>
#include <type.h>
#include <type_arg.h>
#include <binding.h>

using namespace Monitor;


Resource::Resource(Allocator &alloc, Xml_node node,
                   Type_registry_base const &outer_types,
                   Registry<Type_arg> const &type_arguments)
:
	Resource_registry::Element(alloc, string_attribute<Key>(node, "name")),
	_types(outer_types),
	_type_arguments(type_arguments)
{ }


Binding const &Resource::lookup_binding(char const *arg_name) const
{
	return _bindings.lookup(arg_name);
}


Binding const &Resource::lookup_binding(Type_arg const &type_arg) const
{
	Binding const *result = 0;

	_bindings.const_for_each([&] (Binding const &binding) {
		if (binding.belongs_to(type_arg))
			result = &binding; });

	if (result)
		return *result;

	throw Registry<Binding>::Lookup_failed();
}


void Resource::update(Allocator &alloc, Xml_node node,
                      Resource_registry const &outer_resources)
{
	Type_name const type_name = string_attribute<Type_name>(node, "type");

	try         { _type_ptr = &_types.type_by_name(type_name); }
	catch (...) { _type_ptr = 0; }

	/*
	 * Update resource-local types
	 */
	try {
		_types.update(alloc, node); }
	catch (...) {
		PERR("invalid type definition within aggregate");
		throw; }

	_bindings.update(node,
		[]  (Xml_node) { return true; },
		[&] (Xml_node n) { return new (alloc) Binding(alloc, n); },
		[&] (Binding &binding) { Genode::destroy(&alloc, &binding); },
		[&] (Binding &binding, Xml_node node) {

			if (!_type_ptr)
				return;

			/*
			 * Lookup type argument that corresponds to the binding
			 *
			 * Note that for types that are defined at one level (within one
			 * compound resource declaration), the update order is undefined.
			 * For this reason, bindings of resources declared within the
			 * compound resource may refer to type that have not been updated
			 * yet. Because the arguments for those types are unknown at this
			 * point, the lookup for a type argument that matches the binding
			 * may fail.
			 *
			 * However, we do not rely on the binding's matching argument type
			 * for resolving argument requests that refer to resources within
			 * the compound resource. Those are resolved using the
			 * 'Binding::resource_ptr'. The only bindings where the associated
			 * type argument are important, are bindings that refer to
			 * arguments of the compound resource. Those arguments are
			 * guaranteed to be updated prior any of the aggregated resources.
			 */
			Type_arg const *type_arg_ptr = 0;
			try { type_arg_ptr = &_type_ptr->lookup_type_arg(binding.key()); } 
			catch (...) { PERR("lookup_type_arg failed"); }

			binding.bind(alloc, node, type_arg_ptr, outer_resources, _type_arguments);
		});

	/*
	 * Update aggregated resources
	 */
	try {
		_aggregated_resources.update(alloc, node, _types, _type_arguments); }
	catch (...) {
		PERR("invalid resource definition within aggregate");
		throw; }

	/*
	 * Update value
	 *
	 * A value may be supplied either as "value" attribute or as content of
	 * the resource node.
	 */
	_value = Value(node.content_base(), node.content_size());
	if (node.has_attribute("value")) {

		/* do not accept both value attribute and node content */
		if (node.content_size()) {
			PERR("ambigious value of <%s> %s", xml_tag(), type_name.string());
			throw Malformed_resource();
		}
		Xml_node::Attribute const attr = node.attribute("value");
		_value = Value(attr.value_base(), attr.value_size());
	}
}


void Resource::serialize(Xml_generator &xml) const
{
	if (_type_ptr)
		xml.attribute("type", _type_ptr->key().string());

	xml.attribute("name", key().string());

	_bindings.serialize_elements(xml);

	_types.serialize_elements(xml);

	_aggregated_resources.serialize_elements(xml);
}

