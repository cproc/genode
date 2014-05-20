/*
 * \brief  Utility functions for parsing XML
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _XML_UTILS_H_
#define _XML_UTILS_H_

/* Genode includes */
#include <util/string.h>
#include <util/xml_node.h>

/*
 * The utilities are placed in the namespace Genode because they would fit
 * well in the Genode API if they turn out to be useful.
 */
namespace Genode {
	class Missing_xml_attribute { };

	template <typename STRING>
	static inline STRING string_attribute(Xml_node node, char const *attr_name);

	template <typename FUNC>
	static void
	for_each_sub_node(Xml_node, char const *, FUNC const &func);
}


template <typename STRING>
static inline STRING Genode::string_attribute(Xml_node node,
                                              char const *attr_name)
{
	if (!node.has_attribute(attr_name)) {

		PERR("Missing_xml_attribute");
		for (;;);
		throw Missing_xml_attribute();
	}

	char buf[STRING::capacity()];
	node.attribute(attr_name).value(buf, sizeof(buf));

	return STRING(buf);
}


/**
 * Apply functor 'func' to all XML sub nodes of given type
 */
template <typename FUNC>
static void
Genode::for_each_sub_node(Genode::Xml_node node, char const *type,
                          FUNC const &func)
{
	if (!node.has_sub_node(type))
		return;

	for (node = node.sub_node(type); ; node = node.next()) {

		if (node.has_type(type))
			func(node);

		if (node.is_last()) break;
	}
}

#endif /* _XML_UTILS_H_ */
