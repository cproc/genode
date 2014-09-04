#include <base/printf.h>
#include <base/stdint.h>

using namespace Genode;

extern "C" void print_backtrace()
{
	PDBG("*** begin of backtrace ***");

	addr_t *fp;

	asm volatile ("movl %%ebp, %0" : "=r"(fp) : :);

	while (fp && *fp) {
		PDBG("%lx", *(fp + 1));
		fp = (addr_t*)*fp;
	}

	PDBG("*** end of backtrace ***");
}

