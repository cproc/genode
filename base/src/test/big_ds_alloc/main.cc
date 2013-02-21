#include <base/env.h>
#include <dataspace/capability.h>

int main(int argc, char **argv)
{
	enum { DS_SIZE=1024*1024*1024 };
	Genode::env()->ram_session()->alloc(DS_SIZE);

	return 0;
}
