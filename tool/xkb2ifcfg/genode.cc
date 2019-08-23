/*
 * \brief  Genode utility support
 * \author Christian Helmuth <christian.helmuth@genode-labs.com>
 * \date   2019-07-25
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

/* Genode includes */
#include <base/console.h>

#include "util.h"


void Genode::Console::_out_string(char const *str)
{
	if (!str)
		_out_string("<NULL>");
	else
		while (*str) _out_char(*str++);
}


void Genode::Console::printf(const char *format, ...)
{
	va_list list;
	va_start(list, format);
	vprintf(format, list);
	va_end(list);
}


void Genode::Console::vprintf(const char *format, va_list list)
{
	Formatted str(format, list);

	_out_string(str.string());
}
