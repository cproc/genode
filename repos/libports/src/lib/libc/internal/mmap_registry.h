/*
 * \brief  Registry for keeping track of mmapped regions
 * \author Norman Feske
 * \date   2012-08-16
 */

/*
 * Copyright (C) 2012-2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _LIBC__INTERNAL__MMAP_REGISTRY_H_
#define _LIBC__INTERNAL__MMAP_REGISTRY_H_

/* Genode includes */
#include <base/mutex.h>
#include <base/env.h>
#include <base/log.h>
#include <libc/allocator.h>

/* libc includes */
#include <errno.h>
#include <sys/mman.h>

/* libc-internal includes */
#include <internal/plugin.h>
#include <internal/types.h>

namespace Libc {

	class Mmap_registry;

	/**
	 * Return singleton instance of mmap registry
	 */
	Mmap_registry *mmap_registry();
}


class Libc::Mmap_registry
{
	public:

		struct Entry : Avl_node<Entry>
		{
			void   * const start;
			size_t         length;
			Plugin * const plugin;

			Entry(void *start, size_t length, Plugin *plugin)
			: start(start), length(length), plugin(plugin) { }

			bool higher(Entry *other)
			{
				return (other->start > start);
			}

			Entry *find_by_start(void * const start)
			{
				if (start == this->start)
					return this;

				bool side = start > this->start;
				Entry *e = Avl_node<Entry>::child(side);
				return e ? e->find_by_start(start) : 0;
			}
		};

	private:

		/* used during jemalloc initialization */
		Libc::Old_allocator _md_alloc;

		Avl_tree<Mmap_registry::Entry> _tree;

		Mutex mutable _mutex;

		/*
		 * Common for both const and non-const lookup functions
		 */
		template <typename ENTRY>
		static ENTRY *_lookup_by_addr_unsynchronized(ENTRY *curr, void * const start)
		{
			return curr ? curr->find_by_start(start) : 0;
		}

		Entry const *_lookup_by_addr_unsynchronized(void * const start) const
		{
			return _lookup_by_addr_unsynchronized(_tree.first(), start);
		}

		Entry *_lookup_by_addr_unsynchronized(void * const start)
		{
			return _lookup_by_addr_unsynchronized(_tree.first(), start);
		}

	public:

		void insert(void *start, size_t length, Plugin *plugin)
		{
			Mutex::Guard guard(_mutex);

			if (_lookup_by_addr_unsynchronized(start)) {
				warning(__func__, ": mmap region at ", start, " "
				        "is already registered");
				return;
			}

			_tree.insert(new (&_md_alloc) Entry(start, length, plugin));
		}

		Plugin *lookup_plugin_by_addr(void *start) const
		{
			Mutex::Guard guard(_mutex);

			Entry const * const e = _lookup_by_addr_unsynchronized(start);
			return e ? e->plugin : 0;
		}

		bool registered(void *start) const
		{
			Mutex::Guard guard(_mutex);

			return _lookup_by_addr_unsynchronized(start) != 0;
		}

		void remove(void *start)
		{
			Mutex::Guard guard(_mutex);

			Entry *e = _lookup_by_addr_unsynchronized(start);

			if (!e) {
				warning("lookup for address ", start, " "
				        "in in mmap registry failed");
				return;
			}

			_tree.remove(e);
			destroy(&_md_alloc, e);
		}

		void reset()
		{
			while (Entry *e = _tree.first())
				munmap(e->start, e->length);
		}
};


#endif /* _LIBC__INTERNAL__MMAP_REGISTRY_H_ */
