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

#ifndef _BINDING_H_
#define _BINDING_H_

/* local includes */
#include <registry.h>

namespace Monitor {
	class Type;
	class Binding;
	class Resource;
	class Type_arg;
}


class Monitor::Binding : public Registry<Binding>::Element
{
	private:

		/* associated type argument */
		Type_arg const *_type_arg = 0;

		/* assigned resource */
		Resource const *_resource_ptr = 0;
		Type_arg const *_type_arg_ptr = 0;

	public:

		typedef Genode::String<64> Ref_name;

		static constexpr char const *xml_tag() { return "bind"; }
		static constexpr char const *xml_key() { return "arg"; }

		Binding(Allocator &alloc, Xml_node node);

		Type const *type_ptr() const;

		Resource const *resource_ptr() const { return _resource_ptr; }
		Type_arg const *type_arg_ptr() const { return _type_arg_ptr; }

		/*
		 * \param resources  resources accessible within the enclosing
		 *                   aggregate
		 * \param arguments  arguments for enclosing type definition
		 */
		void bind(Allocator &alloc, Xml_node node, Type_arg const *type_arg,
		          Registry<Resource> const &resources,
		          Registry<Type_arg> const &arguments);

		bool belongs_to(Type_arg const &) const;

		void serialize(Xml_generator &xml) const;
};

#endif /* _BINDING_H_ */
