/*
 * \brief  Generic registry of named nodes
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _REGISTRY_H_
#define _REGISTRY_H_

/* Genode includes */
#include <util/list.h>

/* local includes */
#include <types.h>

namespace Monitor {
	class Keyed;
	template <typename> class Registry;
}


/**
 * Common base class of named nodes
 */
class Monitor::Keyed
{
	public:

		typedef Genode::String<64> Key;

	private:

		Key const _key;

	public:

		Keyed(Allocator &, Key const &key) : _key(key) { }

		bool has_key(Key const &key) const { return _key == key; }

		Key key() const { return _key; }
};


template <typename ITEM>
class Monitor::Registry : Noncopyable
{
	public:

		/**
		 * Exception type
		 */
		class Lookup_failed { };

		typedef Genode::List<ITEM> List;

		struct Element : List::Element, Keyed
		{
			Element(Allocator &alloc, Key const &key) : Keyed(alloc, key) { }
		};

		typedef Keyed::Key Element_key;

	private:

		List _list;

		bool _exists(Element_key const &key) const
		{
			try { lookup(key); return true; }
			catch (Lookup_failed) { return false; }
		}

		static Keyed::Key _key(Xml_node node)
		{
			return string_attribute<Keyed::Key>(node, ITEM::xml_key());
		}

		static Xml_node _lookup_sub_node(Xml_node node,
		                                 Element_key const &key)
		{
			char const * const tag = ITEM::xml_tag();

			for (node = node.sub_node(tag); ; node = node.next(tag))
				if (key == _key(node))
					return node;

			throw Xml_node::Nonexistent_sub_node();
		}

		/**
		 * Return true if compound XML node contains a sub node with given name
		 */
		static bool _contains_sub_node_with_key(Xml_node node,
		                                        Element_key const &key)
		{
			try { _lookup_sub_node(node, key); return true; }
			catch (Xml_node::Nonexistent_sub_node) { return false; }
		}

		
		template <typename DESTROY_FN>
		void _destroy_element(ITEM &e, DESTROY_FN const &destroy_fn)
		{
			_list.remove(&e);
			destroy_fn(e);
		}

	public:

		/**
		 * Lookup node by its key
		 *
		 * \throw Lookup_failed
		 */
		ITEM const &lookup(Element_key const &key) const
		{
			for (ITEM const *e = _list.first(); e; e = e->next())
				if (e->has_key(key))
					return *e;

			throw Lookup_failed();
		}

		template <typename FN>
		void const_for_each(FN const &fn) const
		{
			for (ITEM const *e = _list.first(); e; e = e->next()) fn(*e);
		}

		template <typename FN>
		void for_each(FN const &fn)
		{
			for (ITEM *e = _list.first(); e; e = e->next()) fn(*e);
		}

		void serialize_elements(Xml_generator &xml) const
		{
			for (ITEM const *e = _list.first(); e; e = e->next())
				xml.node(ITEM::xml_tag(), [&] () { e->serialize(xml); });
		}

		/**
		 * Update registry according to XML representation
		 *
		 * \param node        compound node
		 * \param ready_fn    functor for testing if item is ready for updating
		 * \param create_fn   functor used for creating a node
		 * \param destroy_fn  functor used for destroying a node
		 * \param update_fn   functor used for updating a node
		 */
		template <typename READY_FN,   typename CREATE_FN,
		          typename DESTROY_FN, typename UPDATE_FN>
		void update(Xml_node   const  compound,
		                   READY_FN   const &ready_fn,
		                   CREATE_FN  const &create_fn,
		                   DESTROY_FN const &destroy_fn,
		                   UPDATE_FN  const &update_fn);
};


template <typename ITEM>
template <typename READY_FN,   typename CREATE_FN,
          typename DESTROY_FN, typename UPDATE_FN>
void Monitor::Registry<ITEM>::update(Xml_node   const  compound,
                                     READY_FN   const &ready_fn,
                                     CREATE_FN  const &create_fn,
                                     DESTROY_FN const &destroy_fn,
                                     UPDATE_FN  const &update_fn)
{
	/*
	 * Remove elements that are no longer present in the XML node
	 */
	for (ITEM *e = _list.first(), *next = 0; e; e = next) {
		next = e->next();
		if (!_contains_sub_node_with_key(compound, e->key()))
			_destroy_element(*e, destroy_fn);
	}

	/*
	 * Add new sub nodes
	 */
	for (unsigned i = 0; i < compound.num_sub_nodes(); i++) {

		Xml_node node = compound.sub_node(i);

		if (!node.has_type(ITEM::xml_tag()))
			continue;

		if (!_exists(_key(node)))
			_list.insert(create_fn(node));
	}

	/*
	 * Update existing elements
	 *
	 * After having removed stage elements, we know that there exists
	 * a matching XML sub node for each remaining list element.
	 */

	Genode::List<ITEM> to_be_updated;

	while (ITEM *item = _list.first()) {
		_list.remove(item);
		to_be_updated.insert(item);
	}

	/* process elements of the 'to_be_updated' list */
	while (ITEM *candidate = to_be_updated.first()) {

		/* lookup candidate that is ready to be updated */
		for (; candidate; candidate = candidate->next())
			if (ready_fn(_lookup_sub_node(compound, candidate->key())))
				break;

		if (!candidate) {

			/* recover from error by removing the stuck items */
			while (ITEM *item = to_be_updated.first()) {
				PERR("dropping <%s %s=\"%s\"> because of possible circular dependency",
				     ITEM::xml_tag(), ITEM::xml_key(), item->key().string());
				_destroy_element(*item, destroy_fn);
				to_be_updated.remove(item);
			}
			break;
		}

		to_be_updated.remove(candidate);
		_list.insert(candidate);

		try {
			update_fn(*candidate, _lookup_sub_node(compound, candidate->key()));
		} catch (...) {
			PERR("dropping invalid <%s> node with key \"%s\"",
			     ITEM::xml_tag(), candidate->key().string());

			/*
			 * Recover from error by removing the faulty item and restart the
			 * update from the beginning.
			 */
			_destroy_element(*candidate, destroy_fn);
		}
	}

	/* reverse list */
	Genode::List<ITEM> tmp;
	while (ITEM *item = _list.first()) {
		_list.remove(item);
		tmp.insert(item);
	}
	_list = tmp;
}

#endif /* _REGISTRY_H_ */
