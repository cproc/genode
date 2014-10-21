#include <base/env.h>
#include <base/printf.h>

using namespace Genode;

int main()
{
	unsigned int i;
	unsigned int mismatch_start = 0;

	enum { MEM_SIZE = 1024*1024*1024 };

	Ram_dataspace_capability ds_cap = env()->ram_session()->alloc(MEM_SIZE);
	char *mem = (char*)env()->rm_session()->attach(ds_cap);

	if (!mem)
		printf("malloc() failed\n");

	printf("writing\n");
	for (i = 0; i < MEM_SIZE; i++) {

		mem[i] = 1;

		register char c = mem[i];
		if (c != 1)
			printf("immediate mismatch\n");
	}

	printf("reading\n");
	for (i = 0; i < MEM_SIZE; i++) {

		register char c = mem[i];

		if (c != 1) {

			printf("mismatch at byte %u (%p): read %d\n", i, &mem[i], c);

			if (mismatch_start == 0)
				mismatch_start = i;

		} else {

			if (mismatch_start != 0) {
				printf("mismatch range: %d bytes\n", i - mismatch_start);
				mismatch_start = 0;
			}

		}
	}

	return 0;
}
