/*
 * \brief  Queue for delayed object destruction
 * \author Christian Prochaska
 * \date   2013-01-03
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NOUX__DESTRUCT_QUEUE_H_
#define _NOUX__DESTRUCT_QUEUE_H_

/* Genode includes */
#include <base/allocator.h>
#include <util/list.h>

class Destruct_queue
{
	public:

		struct Element_base : Genode::List<Element_base>::Element
		{
			virtual void destroy() = 0;
		};

		template <typename T>
		class Element : public Element_base
		{
			private:

				Genode::Allocator *_alloc;

			public:

				Element(Genode::Allocator *alloc) : _alloc(alloc) { }

				virtual ~Element() { };

				void destroy()
				{
					Genode::destroy(_alloc, static_cast<T*>(this));
				}
		};

	private:

		Genode::List<Element_base> _destruct_list;
		Genode::Lock               _destruct_lock;

	public:

		void insert(Element_base *g)
		{
			Genode::Lock::Guard guard(_destruct_lock);
			_destruct_list.insert(g);
		}

		void flush()
		{
			Genode::Lock::Guard guard(_destruct_lock);

			Element_base *g;
			while ((g = _destruct_list.first())) {
				_destruct_list.remove(g);
				g->destroy();
			}
		}
};

#endif /* _NOUX__DESTRUCT_QUEUE_H_ */
