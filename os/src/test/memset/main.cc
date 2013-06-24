#include <base/env.h>
#include <base/printf.h>
#include <os/attached_ram_dataspace.h>

int main(int argc, char **argv)
{
	using namespace Genode;

	enum { SIZE = 512*1024*1024 };

        Attached_ram_dataspace ds(env()->ram_session(), SIZE);
	PDBG("range: %p - %p", ds.local_addr<char>(), ds.local_addr<char>() + SIZE - 1);
	memset(ds.local_addr<char>(), 0, SIZE);

        Attached_ram_dataspace ds2(env()->ram_session(), SIZE);
	PDBG("range: %p - %p", ds2.local_addr<char>(), ds2.local_addr<char>() + SIZE - 1);
	memset(ds2.local_addr<char>(), 0, SIZE);

	return 0;
}
