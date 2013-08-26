#include <base/snprintf.h>

extern "C" int raw_write_str(const char *);

extern "C" void __cyg_profile_func_enter (void *this_fn, void *call_site) __attribute__((no_instrument_function));
extern "C" void __cyg_profile_func_exit  (void *this_fn, void *call_site) __attribute__((no_instrument_function));


struct trace_entry
{
	void *function;
	unsigned long call_count;
};


enum { NUM_TRACE_BUFFER_ENTRIES=1024*1024 };
static struct trace_entry trace_buffer[NUM_TRACE_BUFFER_ENTRIES];
static unsigned int used_entries;


void __cyg_profile_func_enter (void *this_fn, void *call_site)
{
	unsigned int i;
	
	static unsigned long func_enter_count = 0;
	
	func_enter_count++;
	
	if (func_enter_count % 1000 == 0) {
		raw_write_str("\n");
		for (i = 0; i < used_entries; i++) {
			char buf[128];
			Genode::snprintf(buf, sizeof(buf), "%p: %lu\n", trace_buffer[i].function, trace_buffer[i].call_count);
			raw_write_str(buf);
		}
	}

	for (i = 0; i < used_entries; i++) {
		if (trace_buffer[i].function == this_fn)
			break;
	}
	
	if (i == used_entries) {
		if (used_entries == NUM_TRACE_BUFFER_ENTRIES) {
			raw_write_str("trace buffer full\n");
			return;
		}
		trace_buffer[i].function = this_fn;
		used_entries++;
	}
	
	trace_buffer[i].call_count++;
}

extern "C" void __cyg_profile_func_exit  (void *this_fn, void *call_site)
{

}
