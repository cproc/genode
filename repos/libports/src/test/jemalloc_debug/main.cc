#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <jemalloc/jemalloc.h>

uint64_t rdtsc()
{
   uint32_t hi, lo;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
}

int main(int argc, char **argv)
{
	uint64_t allocated = 0;

	printf("start\n");

	uint64_t ts1 = rdtsc();

#if 1
	for (int i = 0; i < 50; i++) {
		for (size_t size = 1; size <= 4096; size++) {
			char *p = (char*)malloc(size);
			if (!p)
				printf("Error: allocation failed");
			allocated += size;
			memset(p, 1, size);
		}
	}
#endif

#if 0
	for (int i = 0; i < 13107200; i++) {

#if 0
		char *p = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
		if (!p)
			printf("Error: allocation failed");
#else
		char *p = (char*)malloc(8);
		if (!p)
			printf("Error: allocation failed");
		memset(p, 1, 8);
#endif
		allocated += 8;
	}
#endif
	uint64_t ts2 = rdtsc();
	uint64_t tsc_khz = 2496000;
//	uint64_t tsc_khz = 3500000;
	uint64_t ts_diff_ms = (ts2 - ts1) / tsc_khz;
	printf("allocated: %lu, milliseconds: %lu\n", allocated, ts_diff_ms);

//	malloc_stats_print(NULL, NULL, NULL);

	return 0;
}
