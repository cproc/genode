#include <base/log.h>

#include "virtual_exception.h"

void Virtual_exception::func()
{
	Genode::log("Virtual_exception::func() called");
}

void thrower()
{
	throw Virtual_exception();
}
