#include <base/printf.h>
#include <base/stdint.h>

using namespace Genode;

void print_backtrace()
{
	PDBG("begin of backtrace");

	addr_t *fp;

	asm volatile ("mov %0, %%fp" : "=r"(fp) : :);

	while (fp && *fp) {
		PDBG("%lx", *fp);
		fp = (addr_t*)*(fp - 1);
	}

	PDBG("end of backtrace");
}

