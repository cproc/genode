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

#ifndef _RESOURCE_H_
#define _RESOURCE_H_

/* local includes */
#include <type_registry.h>
#include <resource_registry.h>
#include <instance.h>
#include <value.h>

namespace Monitor {
	class Resource;
	class Type_arg;
	class Binding;
}


class Monitor::Resource : public Resource_registry::Element
{
	public:

		class Argument;

	private:

		/**
		 * Name space of types visible to the sub tree of the resource
		 */
		Type_registry _types;

		/**
		 * Type of this resource
		 */
		Type const * _type_ptr = 0;

		/**
		 * Resources embedded in the resource
		 */
		Resource_registry _aggregated_resources;

		/**
		 * Type arguments of enclosing type, used to bind resource arguments to
		 * type arguments
		 */
		Registry<Type_arg> const &_type_arguments;

		/**
		 * Bindings of resource arguments to other resources or type arguments
		 */
		Registry<Binding> _bindings;

		Value _value;

	public:

		class Lookup_failed { };
		class Malformed_resource { };

		static constexpr char const *xml_tag() { return "resource"; }
		static constexpr char const *xml_key() { return "name"; }

		Resource(Allocator &, Xml_node,
		         Type_registry_base const &outer_types,
		         Registry<Type_arg> const &type_arguments);

		Type const *type_ptr() const { return _type_ptr; }

		/**
		 * Determine binding to argument with specified name
		 */
		Binding const &lookup_binding(char const *arg_name) const;

		/**
		 * Determine binding that matches the specified type argument
		 *
		 * \throw Lookup_failed
		 */
		Binding const &lookup_binding(Type_arg const &type_arg) const;

		Value value() const { return _value; }

		/*
		 * The 'outer_resources' are passed to the update function to bind the
		 * resources of the parent resource to arguments of the resource scope.
		 */
		void update(Allocator &alloc, Xml_node node,
		            Resource_registry const &outer_resources);

		void serialize(Xml_generator &xml) const;
};

#endif /* _RESOURCE_H_ */
