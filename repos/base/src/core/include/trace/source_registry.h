/*
 * \brief  Registry containing possible sources of tracing data
 * \author Norman Feske
 * \date   2013-08-09
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CORE__INCLUDE__TRACE__SOURCE_REGISTRY_H_
#define _CORE__INCLUDE__TRACE__SOURCE_REGISTRY_H_

#include <util/list.h>
#include <util/string.h>
#include <base/lock.h>
#include <base/trace/types.h>
#include <base/weak_ptr.h>

/* base-internal include */
#include <trace/control.h>

namespace Genode { namespace Trace {
	class Source;
	class Source_owner;
	class Source_registry;
} }


struct Genode::Trace::Source_owner { };


/**
 * Source of tracing data
 *
 * There is one instance per thread.
 */
class Genode::Trace::Source
:
	public Genode::Weak_object<Genode::Trace::Source>,
	public Genode::List<Genode::Trace::Source>::Element
{
	private:

		unsigned      const  _unique_id;
		Session_label const &_label;
		Thread_name   const  _name;
		Control             &_control;
		Dataspace_capability _policy;
		Dataspace_capability _buffer;
		Source_owner  const *_owner;

		static unsigned _alloc_unique_id();

	public:

		Source(Session_label const &label, Thread_name const &name,
		       Control &control)
		:
			_unique_id(_alloc_unique_id()),
			_label(label), _name(name), _control(control), _owner(0)
		{ }


		/*************************************
		 ** Interface used by TRACE service **
		 *************************************/

		Session_label const &label() const { return _label; }
		Thread_name   const &name()  const { return _name; }

		void trace(Dataspace_capability policy, Dataspace_capability buffer)
		{
			_buffer = buffer;
			_policy = policy;
			_control.trace();
		}

		void enable()  { _control.enable(); }
		void disable() { _control.disable(); }

		bool try_acquire(Source_owner const *new_owner)
		{
			if (_owner && _owner != new_owner)
				return false;

			_owner = new_owner;
			return true;
		}

		bool is_owned_by(Source_owner const *owner) { return owner == _owner; }

		void release_ownership(Source_owner const *owner)
		{
			if (is_owned_by(owner))
				_owner = 0;
		}

		bool error()   const { return _control.has_error(); }
		bool enabled() const { return _control.is_enabled(); }


		/***********************************
		 ** Interface used by CPU service **
		 ***********************************/

		Dataspace_capability buffer()    const { return _buffer; }
		Dataspace_capability policy()    const { return _policy; }
		unsigned             unique_id() const { return _unique_id; }
};


/**
 * Registry to tracing sources
 *
 * There is a single instance within core.
 */
class Genode::Trace::Source_registry
{
	private:

		Lock         _lock;
		List<Source> _entries;

	public:

		/***********************************
		 ** Interface used by CPU service **
		 ***********************************/

		void insert(Source *entry)
		{
			Lock::Guard guard(_lock);

			_entries.insert(entry);
		}

		void remove(Source *entry)
		{
			Lock::Guard guard(_lock);
			_entries.remove(entry);
		}


		/*************************************
		 ** Interface used by TRACE service **
		 *************************************/

		template <typename TEST, typename INSERT>
		void export_sources(TEST &test, INSERT &insert)
		{
			for (Source *s = _entries.first(); s; s = s->next())
				if (!test(s->unique_id()))
					insert(s->unique_id(), s->weak_ptr(), s->label(), s->name());
		}

};

#endif /* _CORE__INCLUDE__TRACE__SOURCE_REGISTRY_H_ */
