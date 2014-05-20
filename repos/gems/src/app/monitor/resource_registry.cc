/*
 * \brief  Resource registry
 * \author Norman Feske
 * \date   2014-01-18
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <type.h>
#include <resource_registry.h>
#include <resource.h>
#include <binding.h>
#include <type_arg.h>

using namespace Monitor;


void Resource_registry::update(Allocator &alloc, Xml_node const node,
                               Type_registry_base const &types,
                               Registry<Type_arg> const &type_args)
{
	Registry<Resource> &resources = *this;

	auto ready_fn = [&] (Xml_node node)
	{
		for (unsigned i = 0; i < node.num_sub_nodes(); i++) {

			Xml_node sub_node = node.sub_node(i);

			/* skip tags other than <bind> */
			if (!sub_node.has_type(Binding::xml_tag()))
				continue;

			Binding::Ref_name const ref =
				string_attribute<Binding::Ref_name>(sub_node, "ref");

			/* look if the reference refers to one of our type arguments */
			try {
				type_args.lookup(ref);
				continue; }
			catch (...) { }

			/* check if reference refers to any of the existing siblings */
			try {
				resources.lookup(ref);
				continue; }
			catch (...) { }

			return false;
		}

		/* apparently all <bind> sub nodes are fine */
		return true;
	};

	Registry<Resource>::update(node, ready_fn,
		[&] (Xml_node node) {
			return new (alloc) Resource(alloc, node, types, type_args); },
		[&] (Resource &res) { Genode::destroy(&alloc, &res); },
		[&] (Resource &res, Xml_node node) { res.update(alloc, node, *this); });
}

