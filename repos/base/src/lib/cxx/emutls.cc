/*
 * \brief  TLS support
 * \author Christian Prochaska
 * \date   2019-06-13
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/*
 * XXX: allocated data does not get freed when threads are destroyed
 */

/* Genode includes */
#include <base/heap.h>
#include <base/log.h>
#include <base/thread.h>

using namespace Genode;

/* implemented in 'malloc_free.cc' */
extern Heap &cxx_heap();

static constexpr bool verbose = false;


/*
 * based on the declaration in libgcc's 'emutls.c'
 */
struct __emutls_object
{
  size_t const size;  /* size of the thread-local data */
  size_t const align; /* alignment of the thread-local data */
  void *ptr;          /* AVL tree containing the thread-local data of all
                         threads */
  void const *templ;  /* template for initializing the thread-local data */
};


class Tls_node : public Avl_node<Tls_node>
{
	private:

		void *_address;
		void *_data;

		/* Noncopyable */
		Tls_node(const Tls_node&);
		void operator=(const Tls_node&);

	public:

		Tls_node(void *address, void *data)
		: _address(address), _data(data) { }

		void *data() { return _data; }

		bool higher(Tls_node *other)
		{
			return (other->_address > _address);
		}

		Tls_node *find_by_address(void *address)
		{
			if (address == _address) return this;

			Tls_node *c = child(address > _address);
			return c ? c->find_by_address(address) : nullptr;
		}
};


typedef Avl_tree<Tls_node> Tls_tree;


extern "C" void *__emutls_get_address(void *obj)
{
	static Lock lock;

	Lock::Guard lock_guard(lock);

	__emutls_object *emutls_object = reinterpret_cast<__emutls_object*>(obj);

	if (verbose)
		log(__func__, ": emutls_object: ", emutls_object,
		    ", size: ", emutls_object->size,
	    	", align: ", emutls_object->align,
		    ", ptr: ", emutls_object->ptr,
		    ", templ: ", emutls_object->templ);

	if (!emutls_object->ptr)
		emutls_object->ptr = new (cxx_heap()) Avl_tree<Tls_node>;

	Tls_tree *tls_tree = static_cast<Tls_tree*>(emutls_object->ptr);

	Thread *myself = Thread::myself();

	Tls_node *tls_node = nullptr;

	if (tls_tree->first())
		tls_node = tls_tree->first()->find_by_address(myself);

	if (!tls_node) {

		/* the heap allocates 16-byte aligned */
		if ((16 % emutls_object->align) != 0)
			Genode::warning(__func__, ": cannot ensure alignment of ",
			                emutls_object->align, " bytes");

		void *data = nullptr;
		if (!cxx_heap().alloc(emutls_object->size, &data)) {
			Genode::error(__func__, ": could not allocate thread-local data");
			return nullptr;
		}

		if (emutls_object->templ)
			memcpy(data, emutls_object->templ, emutls_object->size);
		else
			memset(data, 0, emutls_object->size);

		tls_node = new (cxx_heap()) Tls_node(myself, data);

		tls_tree->insert(tls_node);
	}

	return tls_node->data();
}
