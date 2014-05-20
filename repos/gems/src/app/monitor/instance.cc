/*
 * \brief  Resource instantiation
 * \author Norman Feske
 * \date   2014-01-21
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <type.h>
#include <instance.h>
#include <resource.h>
#include <binding.h>
#include <type_arg.h>

using namespace Monitor;


/*******************
 ** Instance_base **
 *******************/

Value Instance_base::value() const { return _resource.value(); }


Instance_base const &Instance_base::arg(char const *arg_name) const
{
	/*
	 * Check if the request reached the root of the instance tree without
	 * resolution.
	 */
	if (!_parent)
		throw Argument_denied();

	return _parent->arg_request_from_child(_resource.lookup_binding(arg_name));
}


/**************
 ** Instance **
 **************/

Instance_base const &Instance::_lookup(Resource const &resource) const
{
	for (Instance_base const *i = _up_to_date.first(); i; i = i->next())
		if (&resource == &i->resource())
			return *i;

	throw Lookup_failed();
}


bool Instance::_bindings_complete(Instance_base const &child) const
{
	struct Invalid_binding { };

	auto check_type_arg_fn = [&] (Type_arg const &type_arg)
	{
		try {
			Binding const &binding = child.resource().lookup_binding(type_arg);

			if (binding.type_arg_ptr())
				return;

			/* check if instance is in the list of up-to-date instances */
			_lookup(*binding.resource_ptr());

		} catch (Registry<Binding>::Lookup_failed) {
			PERR("no valid binding for argument \"%s\" of resource \"%s\"",
			     type_arg.key().string(), child.resource().key().string());
			throw Invalid_binding();
		}
	};

	try {
		child.resource().type_ptr()->const_for_each_arg(check_type_arg_fn);
		return true;
	} catch (...) { }

	return false;
}


void Instance::update(Allocator &alloc)
{
	if (!resource().type_ptr()) {
		PERR("cannot update instance of unknown type");
		return;
	}

	/* temporary list of to-be-updated child instances */
	Genode::List<Instance_base> _to_be_updated;

	Instance const &instance = *this;
	resource().type_ptr()->resources().const_for_each(
		[&] (Resource const &resource) {

			try {
				Instance_base const &existing_instance = _lookup(resource);

				/* mark existing instance to be updated */
				_up_to_date.remove(&existing_instance);
				_to_be_updated.insert(&existing_instance);
			}
			catch (Lookup_failed) {

				/* create new instance */
				Type const *type_ptr = resource.type_ptr();
				if (type_ptr) {
					_to_be_updated.insert(&type_ptr->create_instance(alloc, instance, resource));
				} else {
					PERR("skipping instantiation of \"%s\" of unknown type",
					     resource.key().string());
				}
			}
		});

	/* destroy instances that are no longer part of the resource */
	while (Instance_base *instance = _up_to_date.first()) {

		_up_to_date.remove(instance);

		/* destroy instance */
		if (Type const *type_ptr = instance->resource().type_ptr())
			type_ptr->destroy_instance(alloc, *instance);
	}

	/* update instance in the order that satisfies their inter dependencies */
	while (Instance_base *candidate = _to_be_updated.first()) {

		/* pick candidate whole dependencies are met */
		for (; candidate; candidate = candidate->next())
			if (_bindings_complete(*candidate))
				break;

		if (!candidate) {
			PERR("cyclic dependency within resource \"%s\"",
			     resource().key().string());
			throw Cyclic_dependency();
		}

		_to_be_updated.remove(candidate);
		_up_to_date.insert(candidate);
		candidate->update(alloc);
	}
}


Instance_base const &Instance::arg_request_from_child(Binding const &child_binding) const
{
	/*
	 * Check if argument was bound to one of the resource of our children.
	 */
	if (child_binding.resource_ptr()) {
		try {
			return _lookup(*child_binding.resource_ptr()); }
		catch (Lookup_failed) {
			throw Instance_base::Argument_denied(); }
	}

	/*
	 * Propagate the request towards the root of the instance tree.
	 */

	/*
	 * Check if we reached the root of the instance tree. This should never
	 * happen because it would mean that we created a binding to a non-existing
	 * resource. This should be prevented by the 'Binding' constructor.
	 */
	if (!parent())
		throw Instance_base::Argument_denied();

	Type_arg const *type_arg_ptr = child_binding.type_arg_ptr();
	if (type_arg_ptr) {

		/*
		 * Lookup our own binding that refers to the same 'Type_arg' as the
		 * child binding
		 */
		Binding const &binding = resource().lookup_binding(*type_arg_ptr);
		return parent()->arg_request_from_child(binding);
	}

	/*
	 * This point should never be reached because a binding won't be created
	 * without an associated resource or type argument.
	 */
	throw Instance_base::Argument_denied();
}
