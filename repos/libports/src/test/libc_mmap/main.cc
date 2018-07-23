/*
 * \brief  Test memory allocation with mmap()
 * \author Christian Prochaska
 * \date   2018-07-23
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/log.h>

#include <stdlib.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
	enum { ALLOC_SIZE = 28*1024};
	size_t allocated = 0;

	for (int i = 1; i <= 4000; i++) {

		void *addr = mmap(NULL, ALLOC_SIZE, PROT_NONE, MAP_ANON, -1, 0);

		if (addr != MAP_FAILED) {
			allocated += ALLOC_SIZE;
			Genode::log(i, ": addr: ", addr, ", allocated: ", allocated);
		} else
			Genode::error(i, ": mmap() failed");
	}
}
