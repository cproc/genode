/*
 * \brief  Representation of a constant value
 * \author Norman Feske
 * \date   2014-01-21
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _VALUE_H_
#define _VALUE_H_

#include <types.h>

namespace Monitor { class Value; }


class Monitor::Value
{
	private:

		char const *_base;
		size_t      _size;

	public:

		/**
		 * Default constructor creates invalid value
		 */
		Value() : _base(nullptr), _size(0) { }

		/**
		 * Constructor
		 */
		Value(char const *base, size_t size) : _base(base), _size(size) { }

		bool        is_valid() const { return _base != nullptr; }
		char const *base()     const { return _base; }
		size_t      size()     const { return _size; }
};

#endif /* _VALUE_H_ */
