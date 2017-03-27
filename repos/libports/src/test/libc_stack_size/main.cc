/*
 * \brief  Test for overriding the stack size of the libc main thread
 * \author Christian Prochaska
 * \date   2017-03-27
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/log.h>
#include <libc/component.h>

int main(int argc, char **argv) { }

Genode::size_t Libc::Component::stack_size()
{
	Genode::log("Libc::Component::stack_size() called");
	return 1024*1024;
}
