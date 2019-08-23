/*
 * \brief  Libxkbcommon-based keyboard-layout generator
 * \author Christian Helmuth <christian.helmuth@genode-labs.com>
 * \date   2019-08-16
 *
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _UTIL_H_
#define _UTIL_H_

/* Linux includes */
#include <cstdio>
#include <cstdlib>


struct Formatted
{
	char *_string;

	Formatted(char const *format, va_list list)
	{
		::vasprintf(&_string, format, list);
	}

	Formatted(char const *format, ...)
	{
		va_list list;
		va_start(list, format);
		::vasprintf(&_string, format, list);
		va_end(list);
	}

	~Formatted()
	{
		::free(_string);
	}

	char const * string() const { return _string; }
};


#endif /* _UTIL_H_ */
