/*
 * \brief  Registry containing tracing policy modules
 * \author Norman Feske
 * \date   2013-08-12
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CORE__INCLUDE__TRACE__POLICY_REGISTRY_H_
#define _CORE__INCLUDE__TRACE__POLICY_REGISTRY_H_

/* Genode includes */
#include <base/lock.h>
#include <util/list.h>

namespace Genode { namespace Trace {
	class Policy_owner;
	class Policy;
	class Policy_registry;
} }


class Genode::Trace::Policy_owner { };


class Genode::Trace::Policy : public Genode::List<Genode::Trace::Policy>::Element
{
	friend class Policy_registry;

	private:

		Policy_owner  const &_owner;
		Allocator           &_md_alloc;
		Policy_id     const  _id;
		Dataspace_capability _ds;
		size_t        const  _size;

		/**
		 * Constructor
		 *
		 * \param md_alloc  allocator that holds the 'Policy' object
		 */
		Policy(Policy_owner const &owner, Policy_id const id,
		       Allocator &md_alloc, Dataspace_capability ds, size_t size)
		:
			_owner(owner), _md_alloc(md_alloc), _id(id), _ds(ds), _size(size)
		{ }

		Allocator &md_alloc() { return _md_alloc; }

		bool is_owned_by(Policy_owner const &owner) const
		{
			return &_owner == &owner;
		}

		bool has_id(Policy_id id) const { return id == _id; }

	public:

		Dataspace_capability dataspace() const { return _ds; }
		size_t               size()      const { return _size; }
};


/**
 * Global policy registry
 */
class Genode::Trace::Policy_registry
{

	private:

		Lock         _lock;
		List<Policy> _policies;

		Policy *_unsynchronized_lookup(Policy_owner const &owner, Policy_id id)
		{
			for (Policy *p = _policies.first(); p; p++)
				if (p->is_owned_by(owner) && p->has_id(id))
					return p;

			throw Nonexistent_policy();
		}

	public:

		void insert(Policy_owner const &owner, Policy_id const id,
		            Allocator &md_alloc, Dataspace_capability ds, size_t size)
		{
			Lock::Guard guard(_lock);

			Policy *policy = new (&md_alloc) Policy(owner, id, md_alloc, ds, size);
			_policies.insert(policy);
		}

		void remove(Policy_owner &owner, Policy_id id)
		{
			Lock::Guard guard(_lock);

			for (Policy *p = _policies.first(); p; p++)
				if (p->is_owned_by(owner) && p->has_id(id))
					destroy(&p->md_alloc(), p);
		}

		void destroy_policies_owned_by(Policy_owner const &owner)
		{
			Lock::Guard guard(_lock);

			for (Policy *p = _policies.first(); p; p++)
				if (p->is_owned_by(owner))
					destroy(&p->md_alloc(), p);
		}

		Dataspace_capability dataspace(Policy_owner &owner, Policy_id id)
		{
			Lock::Guard guard(_lock);

			return _unsynchronized_lookup(owner, id)->dataspace();
		}

		size_t size(Policy_owner &owner, Policy_id id)
		{
			Lock::Guard guard(_lock);

			return _unsynchronized_lookup(owner, id)->size();
		}
};

#endif /* _CORE__INCLUDE__TRACE__POLICY_REGISTRY_H_ */
