#include <base/printf.h>

#include <stdlib.h>

extern "C" void wait_for_continue();

int main(int argc, char *argv[])
{
	long *addr;
	PINF("calling malloc()");
	addr = (long*)malloc(sizeof(long));
	PINF("calling free()");
	free(addr);
	PINF("calling new");
	//wait_for_continue();
	addr = new long;
	PINF("calling delete");
	delete addr;
	return 0;
}
