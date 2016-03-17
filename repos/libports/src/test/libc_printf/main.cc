#include <base/printf.h>
#include <util/string.h>

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	static char buf[512];
	memset(buf, 'x', sizeof(buf) - 1);

	Genode::printf("\n----- Genode::printf() -----\n\n");

	Genode::printf("%s\n", buf);

	Genode::printf("\n----- libc printf() -----\n\n");

	printf("%s\n", buf);

	return 0;
}
