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

#ifndef _TYPE_REGISTRY_H_
#define _TYPE_REGISTRY_H_

/* Genode includes */
#include <util/list.h>

/* local includes */
#include <registry.h>
#include <type_name.h>

namespace Monitor {
	class Type_registry_base;
	class Type_registry;
	class Type;
	class Resource;
}


/**
 * Common base class of builtin and generic resource types
 */
struct Monitor::Type_registry_base : Registry<Type>
{
	/**
	 * Exception type
	 */
	typedef Registry<Type>::Lookup_failed Type_lookup_failed;

	/**
	 * Lookup type representation by its name
	 *
	 * \throw Type_lookup_failed
	 */
	virtual Type const &type_by_name(Type_name const &name) const
	{
		return Registry<Type>::lookup(name);
	}
};


/**
 * Registry for generic resource types
 */
class Monitor::Type_registry : public Type_registry_base
{
	private:

		Type_registry_base const &_outer_types;

	public:

		Type const &type_by_name(Type_name const &name) const override
		{
			try {
				/* try to look up the type in local scope */
				return Type_registry_base::lookup(name); }

			catch (Lookup_failed) {

				/* look up the type in the parent's scope */
				return _outer_types.type_by_name(name); }
		}

		void update(Allocator &alloc, Xml_node);

		Type_registry(Type_registry_base const &ot) : _outer_types(ot) { }
};

#endif /* _TYPE_REGISTRY_H_ */
