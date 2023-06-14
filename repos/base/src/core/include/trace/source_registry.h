/*
 * \brief  Registry containing possible sources of tracing data
 * \author Norman Feske
 * \date   2013-08-09
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__INCLUDE__TRACE__SOURCE_REGISTRY_H_
#define _CORE__INCLUDE__TRACE__SOURCE_REGISTRY_H_

/* Genode includes */
#include <util/list.h>
#include <base/mutex.h>
#include <base/trace/types.h>
#include <base/weak_ptr.h>

/* base-internal include */
#include <base/internal/trace_control.h>

/* core-internal includes */
#include <types.h>

namespace Core { namespace Trace {

	using namespace Genode::Trace;

	class Source;
	class Source_owner;
	class Source_registry;

	/**
	 * Return singleton instance of trace-source registry
	 */
	Source_registry &sources();
} }


struct Core::Trace::Source_owner { };


/**
 * Source of tracing data
 *
 * There is one instance per thread.
 */
class Core::Trace::Source
:
	public Weak_object<Source>, public List<Source>::Element
{
	public:

		struct Info
		{
			Session_label      label;
			Thread_name        name;
			Execution_time     execution_time;
			Affinity::Location affinity;
		};

		/**
		 * Interface for querying trace-source information
		 */
		struct Info_accessor : Interface
		{
			virtual Info trace_source_info() const = 0;
		};

		/**
		 * Interface for starting a thread
		 */
		struct Thread_starter : Interface
		{
			virtual void start_by_trace_monitor() = 0;
		};

	private:

		unsigned            const  _unique_id;
		Info_accessor       const &_info;
		Control                   &_control;
		Dataspace_capability       _policy { };
		Dataspace_capability       _buffer { };
		Source_owner        const *_owner_ptr = nullptr;
		Thread_starter            *_thread_starter { };
		Signal_context_capability  _trace_start_sigh { };

		static unsigned _alloc_unique_id();

		/*
		 * Noncopyable
		 */
		Source(Source const &);
		Source &operator = (Source const &);

	public:

		Source(Info_accessor const &info, Control &control)
		:
			_unique_id(_alloc_unique_id()), _info(info), _control(control)
		{ }

		~Source()
		{
			/* invalidate weak pointers to this object */
			lock_for_destruction();
		}

		/*************************************
		 ** Interface used by TRACE service **
		 *************************************/

		Info const info() const { return _info.trace_source_info(); }

		void trace(Dataspace_capability policy, Dataspace_capability buffer)
		{
Genode::log("trace()");
			_buffer = buffer;
			_policy = policy;
			_control.trace();
#if 0
			if (_trace_start_sigh.valid()) {
Genode::log("trace(): submitting signal");
				Signal_transmitter(_trace_start_sigh).submit();
			}
#endif
			if (_thread_starter)
				_thread_starter->start_by_trace_monitor();
		}

		void enable()  { _control.enable(); }
		void disable() { _control.disable(); }

		bool try_acquire(Source_owner const &new_owner)
		{
			if (_owner_ptr && _owner_ptr != &new_owner)
				return false;

			_owner_ptr = &new_owner;
			return true;
		}

		bool owned_by(Source_owner const &owner) { return &owner == _owner_ptr; }

		void release_ownership(Source_owner const &owner)
		{
			if (owned_by(owner))
				_owner_ptr = nullptr;
		}

		bool error()   const { return _control.has_error(); }
		bool enabled() const { return _control.enabled(); }


		/***********************************
		 ** Interface used by CPU service **
		 ***********************************/

		Dataspace_capability buffer()    const { return _buffer; }
		Dataspace_capability policy()    const { return _policy; }
		unsigned             unique_id() const { return _unique_id; }

		void thread_starter(Thread_starter *thread_starter)
		{
			_thread_starter = thread_starter;
		}

		void trace_start_sigh(Signal_context_capability sigh)
		{
Genode::log("trace_start_sigh()");
			_trace_start_sigh = sigh;

			if (_trace_start_sigh.valid() && _control.to_be_enabled()) {
Genode::log("trace_start_sigh(): submitting signal");
				Signal_transmitter(_trace_start_sigh).submit();
			}
		}
};


/**
 * Registry to tracing sources
 *
 * There is a single instance within core.
 */
class Core::Trace::Source_registry
{
	public:

		struct Observer : List<Observer>::Element
		{
			Signal_context_capability sigh { };
		};

	private:

		Mutex          _mutex     { };
		List<Source>   _entries   { };
		List<Observer> _observers { };

		void _notify_observers()
		{
			for (Observer *observer = _observers.first();
			     observer; observer = observer->next())
				Signal_transmitter(observer->sigh).submit();
		}

	public:

		/***********************************
		 ** Interface used by CPU service **
		 ***********************************/

		void insert(Source *entry)
		{
			Mutex::Guard guard(_mutex);

			_entries.insert(entry);
			_notify_observers();
		}

		void remove(Source *entry)
		{
			Mutex::Guard guard(_mutex);

			_entries.remove(entry);
			_notify_observers();
		}


		/*************************************
		 ** Interface used by TRACE service **
		 *************************************/

		template <typename TEST, typename INSERT>
		void export_sources(TEST &test, INSERT &insert)
		{
			for (Source *s = _entries.first(); s; s = s->next())
				if (!test(s->unique_id())) {
					Source::Info const info = s->info();
					insert(s->unique_id(), s->weak_ptr(), info.label, info.name);
				}
		}

		void add_observer(Observer *observer)
		{
			_observers.insert(observer);

			/* send initial signal */
			Signal_transmitter(observer->sigh).submit();
		}

		void remove_observer(Observer *observer)
		{
			_observers.remove(observer);
		}
};

#endif /* _CORE__INCLUDE__TRACE__SOURCE_REGISTRY_H_ */
