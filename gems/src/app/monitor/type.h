/*
 * \brief  Resource type
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _TYPE_H_
#define _TYPE_H_

/* Genode includes */
#include <util/list.h>

/* local includes */
#include <resource_registry.h>

namespace Monitor {
	class Type;
	class Type_registry_base;
	class Aggregate;
	class Type_arg;
	class Instance_base;
}


class Monitor::Type : public Registry<Type>::Element
{
	public:

		static constexpr char const *xml_tag() { return "type"; }
		static constexpr char const *xml_key() { return "name"; }

	private:

		Registry<Type_arg> _arguments;

		/**
		 * Aggregated resources
		 */
		Resource_registry _resources;

	public:

		Type(Allocator &alloc, Key const &key);

		/**
		 * Return aggregated resources
		 */
		Registry<Resource> const &resources() const { return _resources; }

		/**
		 * Factory function for creating an instance of the type
		 *
		 * This function virtual to enable builtin type to customize the
		 * creation of instances.
		 */
		virtual Instance_base &create_instance(Allocator &alloc,
		                                       Instance_base const &parent,
		                                       Resource const &resource) const;

		/**
		 * Factory function for destroying an instance
		 */
		virtual void destroy_instance(Allocator &alloc,
		                              Instance_base const &instance) const;

		Type_arg const &lookup_type_arg(Keyed::Key const &) const;

		template <typename FN>
		void const_for_each_arg(FN const &fn) const { _arguments.const_for_each(fn); }

		/**
		 * Update type
		 *
		 * The 'outer_types' argument is the type name space to be passed to an
		 * resources embedded in the type.
		 */
		void update(Allocator &, Xml_node, Type_registry_base const &outer_types);

		virtual void serialize(Xml_generator &xml) const;

		virtual void destroy(Allocator &alloc) { Genode::destroy(&alloc, this); }
};

#endif /* _TYPE_H_ */
