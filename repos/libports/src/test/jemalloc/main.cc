#include <base/log.h>
#include <trace/timestamp.h>

#include <stdlib.h>
#include <jemalloc/jemalloc.h>


int main(int argc, char **argv)
{
	uint64_t allocated = 0;

	uint64_t ts1 = Genode::Trace::timestamp();

	for (int i = 0; i < 50; i++) {
		for (size_t size = 1; size <= 4096; size++) {
			void *p = jemalloc(size);
			if (!p)
				Genode::error("allocation failed");
			allocated += size;
		}
	}

	uint64_t ts2 = Genode::Trace::timestamp();
	uint64_t ts_diff_ms = (ts2 - ts1) / 2496000;
	Genode::log("allocated: ", allocated, ", milliseconds: ", ts_diff_ms);

	//jemalloc_stats_print(NULL, NULL, NULL);

	return 0;
}
