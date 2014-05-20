/*
 * \brief  Representation of type argument
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _TYPE_ARG_H_
#define _TYPE_ARG_H_

/* local includes */
#include <registry.h>

namespace Monitor {
	class Type_arg;
	class Type;
	class Type_registry_base;
}


class Monitor::Type_arg : public Registry<Type_arg>::Element
{
	private:

		Type const * _type_ptr = 0;

	public:

		static constexpr char const *xml_tag() { return "arg"; }
		static constexpr char const *xml_key() { return "name"; }

		Type_arg(Allocator &alloc, Xml_node node);

		void update(Allocator &, Xml_node, Type_registry_base const &);

		Type const *type_ptr() const;

		void serialize(Xml_generator &xml) const;
};

#endif /* _TYPE_ARG_H_ */
