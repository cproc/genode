/*
 * \brief  Common interface for resource instances
 * \author Norman Feske
 * \date   2014-01-18
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

/* Genode includes */
#include <util/list.h>

/* local includes */
#include <resource.h>
#include <binding.h>
#include <value.h>

namespace Monitor {
	class Instance_base;
	class Instance;
	class Resource;
}


/*
 * 'Instance_base' is element of 'Instance::Pool::_list'
 */
class Monitor::Instance_base : public Genode::List<Instance_base>::Element
{
	private:

		Instance_base const *_parent;
		Resource      const &_resource;

	public:

		/**
		 * Exception types
		 */
		struct Cyclic_dependency { };

		/**
		 * Constructor used for normal instances
		 */
		Instance_base(Instance_base const &parent, Resource const &resource)
		:
			_parent(&parent), _resource(resource)
		{ }

		/**
		 * Constructor used for initial instances that have no parent
		 */
		Instance_base(Resource const &res) : _parent(0), _resource(res) { }

		/**
		 * \throw Cyclic_dependency
		 */
		virtual void update(Allocator &alloc) = 0;

		Resource const &resource() const { return _resource; }

		Value value() const;

		class Argument_denied { };
		class Argument_has_wrong_type { };

		template <typename... PATH>
		Instance_base const &arg(char const *arg_name, PATH... path...) const
		{
			return arg(arg_name).arg(path...);
		}

		Instance_base const &arg(char const *arg_name) const;

		Instance_base const *parent() const { return _parent; }

		virtual Instance_base const &arg_request_from_child(Binding const &) const
		{
			/*
			 * This function should be called from child instances only. Since
			 * 'Instance_base' has no children, we do not expect it to be
			 * called.
			 */
			class Unexpected_call_of_arg_request_from_child { };
			throw Unexpected_call_of_arg_request_from_child();
		}

		template <typename ARG_INSTANCE, typename... PATH>
		ARG_INSTANCE const &typed_arg(PATH... path) const
		{
			Instance_base const &result = arg(path...);
			ARG_INSTANCE const *typed_result = dynamic_cast<ARG_INSTANCE const *>(&result);

			if (typed_result)
				return *typed_result;

			throw Argument_has_wrong_type();
		}
};


/**
 * Generic instance, which is a compound of other instances
 */
class Monitor::Instance : public Instance_base
{
	private:

		Genode::List<Instance_base> _up_to_date;

		class Lookup_failed { };

		Instance_base const &_lookup(Resource const &resource) const;

		bool _bindings_complete(Instance_base const &child) const;

	public:

		/**
		 * Constructor used for normal instances
		 */
		Instance(Allocator &, Instance_base const &parent, Resource const &resource)
		: Instance_base(parent, resource) { }

		/**
		 * Constructor used for statically created instances
		 */
		Instance(Allocator &, Resource const &resource)
		: Instance_base(resource) { }

		void update(Allocator &alloc) override;

		/**
		 * Called by a child instance at the parent instance to query an
		 * instance argument.
		 */
		Instance_base const &arg_request_from_child(Binding const &) const override;
};

#endif /* _INSTANCE_H_ */
