#include <base/log.h>

#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
	enum { ALLOC_SIZE = 32*1024 };
	size_t allocated = 0;

	for (int i = 0; i < 1000; i++) {

		void *addr = mmap(NULL, ALLOC_SIZE, PROT_NONE, MAP_ANON, -1, 0);

		if (addr != MAP_FAILED) {
			allocated += ALLOC_SIZE;
			Genode::log("addr: ", addr, ", allocated: ", allocated);
		} else
			Genode::error("map failed");
	}
}
