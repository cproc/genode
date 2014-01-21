/*
 * \brief  Registry of top-level resource types
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* local includes */
#include <top_level_type_registry.h>
#include <builtin_types.h>
#include <type_name.h>


using namespace Monitor;


static Genode::Xml_node top_level_types_xml()
{
	extern char _binary_top_level_types_xml_start,
	            _binary_top_level_types_xml_end;

	return Genode::Xml_node(&_binary_top_level_types_xml_start,
	                        &_binary_top_level_types_xml_end
	                      - &_binary_top_level_types_xml_start);
}


struct Factory_base;
typedef Genode::List<Factory_base> Factory_list;


struct Factory_base : Factory_list::Element
{
	Type_name const name;

	virtual Type *create(Allocator &alloc, Type::Key const &key) const = 0;

	Factory_base(Type_name const &name, Factory_list &list)
	: name(name) { list.insert(this); }
};


template <typename BUILTIN_TYPE>
struct Factory : Factory_base
{
	Type *create(Allocator &alloc, Type::Key const &key) const override
	{
		return new (alloc) BUILTIN_TYPE(alloc, key);
	}

	Factory(Factory_list &list) : Factory_base(BUILTIN_TYPE::name(), list) { }
};


static Type *create_builtin_type(Allocator &alloc, Type::Key const &key)
{
	/**
	 * Factories for creating built-in types
	 */
	struct Factories : Factory_list
	{
		Factory<Builtin_string_type>         string         = { *this };
		Factory<Builtin_parent_type>         parent         = { *this };
		Factory<Builtin_service_type>        service        = { *this };
		Factory<Builtin_parent_service_type> parent_service = { *this };
	};
	static Factories const factories;

	for (auto *factory = factories.first(); factory; factory = factory->next())
		if (factory->name == key)
			return factory->create(alloc, key);

	PERR("builtin type \"%s\" does not exist", key.string());
	throw Top_level_type_registry::Builtin_type_does_not_exist();
}


static Type *create_generic_type(Allocator &alloc, Type::Key const &key)
{
	return new (alloc) Type(alloc, key);
}


Top_level_type_registry::Top_level_type_registry(Allocator &alloc)
{
	auto create_fn = [&] (Xml_node node)
	{
		try {
			Type::Key const key = string_attribute<Type::Key>(node, Type::xml_key());

			if (node.has_attribute("builtin")
			 && node.attribute("builtin").has_value("yes"))
				return create_builtin_type(alloc, key);

			/*
			 * The helper function is used to resolve const aliasing of return type
			 * in the 'create_fn' lamda
			 */
			return create_generic_type(alloc, key);

		} catch (Xml_node::Nonexistent_attribute) {
			PERR("unnamed <type>");
			throw;
		}
	};

	auto ready_fn = [&] (Xml_node) { return true; };

	/* top-level types are never destroyed */
	auto destroy_fn = [&] (Type &type) { };

	Type_registry_base const &top_level_types = *this;

	auto update_fn = [&] (Type &type, Xml_node node)
	{
		type.update(alloc, node, top_level_types);
	};

	update(top_level_types_xml(), ready_fn, create_fn, destroy_fn, update_fn);
}
