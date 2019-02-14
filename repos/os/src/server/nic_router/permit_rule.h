/*
 * \brief  Rules for permitting ports in the context of a transport rule
 * \author Martin Stein
 * \date   2016-08-19
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _PERMIT_RULE_H_
#define _PERMIT_RULE_H_

/* local includes */
#include <avl_tree.h>

/* Genode includes */
#include <util/avl_tree.h>
#include <net/port.h>

namespace Genode {

	class Output;
	class Xml_node;
}

namespace Net {

	class Interface;
	class Domain;
	class Domain_tree;

	class Permit_rule;
	class Permit_any_rule;
	class Permit_single_rule;
	class Permit_single_rule_tree;
}


struct Net::Permit_rule : public Genode::Interface
{
	friend class Interface;

	private:

		Domain &_domain;

	public:

		Permit_rule(Domain &domain) : _domain(domain) { };


		/*********
		 ** log **
		 *********/

		virtual void print(Genode::Output &output) const = 0;


		/***************
		 ** Accessors **
		 ***************/

		Domain &domain() const { return _domain; }
};


struct Net::Permit_any_rule : Permit_rule
{
	private:

		static Domain &_find_domain(Domain_tree            &domains,
		                            Genode::Xml_node const  node);

	public:

		struct Invalid : Genode::Exception { };

		Permit_any_rule(Domain_tree &domains, Genode::Xml_node const node);


		/*********
		 ** log **
		 *********/

		void print(Genode::Output &output) const override;
};


class Net::Permit_single_rule : public  Permit_rule,
                                private Genode::Avl_node<Permit_single_rule>
{
	friend class Genode::Avl_node<Permit_single_rule>;
	friend class Genode::Avl_tree<Permit_single_rule>;
	friend class Avl_tree<Permit_single_rule>;
	friend class Net::Permit_single_rule_tree;

	private:

		Port const _port;

		static Domain &_find_domain(Domain_tree            &domains,
		                            Genode::Xml_node const  node);

	public:

		struct Invalid : Genode::Exception { };

		Permit_single_rule(Domain_tree            &domains,
		                   Genode::Xml_node const  node);

		Permit_single_rule const &find_by_port(Port const port) const;


		/*********
		 ** log **
		 *********/

		void print(Genode::Output &output) const override;


		/**************
		 ** Avl_node **
		 **************/

		bool higher(Permit_single_rule *rule);


		/***************
		 ** Accessors **
		 ***************/

		Port port() const { return _port; }
};


struct Net::Permit_single_rule_tree : private Avl_tree<Permit_single_rule>
{
	friend class Transport_rule;

	struct No_match : Genode::Exception { };

	void insert(Permit_single_rule *rule)
	{
		Genode::Avl_tree<Permit_single_rule>::insert(rule);
	}

	using Genode::Avl_tree<Permit_single_rule>::first;

	Permit_single_rule const &find_by_port(Port const port) const;
};

#endif /* _PERMIT_RULE_H_ */
