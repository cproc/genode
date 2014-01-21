/*
 * \brief  Built-in resource types
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _BUILTIN_TYPES_H_
#define _BUILTIN_TYPES_H_

#include <types.h>
#include <instance.h>
#include <type.h>

namespace Monitor {
	struct Builtin_type_base;
	template <typename> struct Builtin_type;
}


namespace Monitor { class Empty_instance; }
struct Monitor::Empty_instance : Instance_base
{
	Empty_instance(Instance_base const &parent, Resource const &resource)
	: Instance_base(parent, resource) { }

	void update(Allocator &) override { }
};


template <typename INSTANCE = Monitor::Empty_instance>
struct Monitor::Builtin_type : Type
{
	Builtin_type(Allocator &alloc, Key const &key)
	: Type(alloc, key) { }

	void serialize(Xml_generator &xml) const override
	{
		xml.attribute("builtin", "yes");
		Type::serialize(xml);
	}

	Instance_base &create_instance(Allocator           &alloc,
	                               Instance_base const &parent,
	                               Resource      const &resource) const override
	{
		return *new (alloc) INSTANCE(parent, resource);
	}
};


/************
 ** string **
 ************/

namespace Monitor { class String_instance; }

struct Monitor::String_instance : public Instance_base
{
	bool up_to_date = false;

	String_instance(Instance_base const &parent, Resource const &resource)
	:
		Instance_base(parent, resource)
	{
		PLOG("String_instance construct %s", resource.key().string());
	}

	void print() const
	{
		Genode::String<128> string(value().base(), value().size());
		PINF("String_instance::print: value=\"%s\" up_to_date=%d",
		     string.string(), up_to_date);
	}

	void update(Allocator &) override
	{
		PLOG("String_instance update");
		up_to_date = true;
	}
};


namespace Monitor { class Builtin_string_type; }

class Monitor::Builtin_string_type : public Builtin_type<String_instance>
{
	public:

		static constexpr char const *name() { return "string"; }

		Builtin_string_type(Allocator &alloc, Key const &key)
		:
			Builtin_type(alloc, key)
		{ }
};


/************
 ** parent **
 ************/

namespace Monitor { class Builtin_parent_type; }


class Monitor::Builtin_parent_type : public Builtin_type<>
{
	public:

		static constexpr char const *name() { return "parent"; }

		Builtin_parent_type(Allocator &alloc, Key const &key)
		:
			Builtin_type(alloc, key)
		{ }
};


/*************
 ** service **
 *************/

namespace Monitor { class Builtin_service_type; }


class Monitor::Builtin_service_type : public Builtin_type<>
{
	public:

		static constexpr char const *name() { return "service"; }

		Builtin_service_type(Allocator &alloc, Key const &key)
		:
			Builtin_type(alloc, key)
		{ }
};


/********************
 ** parent_service **
 ********************/

namespace Monitor { class Parent_service_instance; }

struct Monitor::Parent_service_instance : public Instance_base
{
	Parent_service_instance(Instance_base const &parent, Resource const &resource)
	: Instance_base(parent, resource)
	{
		PLOG("Parent_service_instance construct %s", resource.key().string());
	}

	void update(Allocator &) override
	{
		PINF("Parent_service_instance update");

		String_instance const &string =
			typed_arg<String_instance>("service_name_a", "string_a");

		string.print();

		PINF("Parent_service_instance request foo_a");

		String_instance const &string2 = typed_arg<String_instance>("foo_a");
		string2.print();
	}
};


namespace Monitor { class Builtin_parent_service_type; }

class Monitor::Builtin_parent_service_type : public Builtin_type<Parent_service_instance>
{
	public:

		static constexpr char const *name() { return "parent_service"; }

		Builtin_parent_service_type(Allocator &alloc, Key const &key)
		:
			Builtin_type(alloc, key)
		{ }
};


#endif /* _BUILTIN_TYPES_H_ */
