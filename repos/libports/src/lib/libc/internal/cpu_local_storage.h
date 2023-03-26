/*
 * \brief  CPU-local storage
 * \author Christian Prochaska
 * \date   2023-03-27
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _LIBC__INTERNAL__CPU_LOCAL_STORAGE_H_
#define _LIBC__INTERNAL__CPU_LOCAL_STORAGE_H_

/* libc includes */
#include <libc/allocator.h>

/* libc-internal includes */
#include <internal/timer.h>


namespace Libc {

	class  Cpu_local_storage_registry;
	struct Cpu_local_storage;

	/**
	 * Return singleton instance of CPU-local storage registry
	 */
	Cpu_local_storage_registry &cpu_local_storage_registry();
}


class Libc::Cpu_local_storage : public Genode::Avl_node<Libc::Cpu_local_storage>
{
	private:

		Genode::Env &_env;

		Genode::Affinity::Location _location;

		/* One timer connection per CPU. */
		Genode::Constructible<Libc::Timer> _timer;

		/*
		 * The '_timer' is constructed by whatever thread (main thread
		 * or pthread) that uses a time-related function first. Hence,
		 * the construction must be protected by a mutex.
		 */
		Genode::Mutex _timer_construct_mutex;

	public:

		Cpu_local_storage(Genode::Env &env, Genode::Affinity::Location location)
		: _env(env), _location(location) { }


		Genode::Affinity::Location location() const
		{
			return _location;
		}


		Libc::Timer &timer()
		{
			if (!_timer.constructed()) {
				Genode::Mutex::Guard guard(_timer_construct_mutex);
				if (!_timer.constructed())
					_timer.construct(_env);
			}

			return *_timer;
		}

		/* Avl_node interface */

		bool higher(Cpu_local_storage *other);

		Cpu_local_storage *find_by_location(Genode::Affinity::Location location);
};


class Libc::Cpu_local_storage_registry
{
	private:

		Genode::Env                               &_env;
		Genode::Avl_tree<Libc::Cpu_local_storage>  _tree  { };
		Mutex                                      _mutex { };
		/* used during jemalloc initialization */
		Libc::Old_allocator                        _alloc { };

	public:

		Cpu_local_storage_registry(Genode::Env &env) : _env(env) { }

		Cpu_local_storage &get(Genode::Affinity::Location location)
		{
			Mutex::Guard guard(_mutex);

			Cpu_local_storage *cls =
				_tree.first() ?
				_tree.first()->find_by_location(location) :
				nullptr;

			if (!cls) {
				cls = new (_alloc) Cpu_local_storage(_env, location);
				_tree.insert(cls);
			}

			return *cls;
		}
};

#endif /* _LIBC__INTERNAL__CPU_LOCAL_STORAGE_H_ */
