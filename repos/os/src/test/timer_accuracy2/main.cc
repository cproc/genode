#include <timer_session/connection.h>
#include <trace/timestamp.h>

enum { STACK_SIZE = 2*1024*sizeof(Genode::addr_t) };

struct Load_thread : Genode::Thread<STACK_SIZE>
{
	Load_thread() : Genode::Thread<STACK_SIZE>("load_thread") { }

	void entry()
	{
		PDBG("entry()");
		while(1);
	}
};

int main()
{
	enum { TEST_COUNT = 10000 };

	static Timer::Connection timer;
#if 0
	static Load_thread load_thread;
	load_thread.start();
#endif
	Genode::uint64_t sum = 0;
	Genode::uint64_t min = ~0ULL;
	Genode::uint64_t max = 0;

	for (int i = 0; i < TEST_COUNT; i++) {

		Genode::uint64_t start = Genode::Trace::timestamp();
		timer.msleep(1);
		Genode::uint64_t stop = Genode::Trace::timestamp();
		Genode::uint64_t diff = stop - start;

		sum += diff;

		if (diff < min)
			min = diff;
		else if (diff > max)
			max = diff;
	}

	PDBG("min: %llu ns, max: %llu ns, avg: %llu ns",
	     (min * 1000 / 2400), (max * 1000 / 2400), (sum / TEST_COUNT) * 1000 / 2400);

	return 0;
}
