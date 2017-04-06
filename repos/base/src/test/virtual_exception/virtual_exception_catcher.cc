#include "virtual_exception.h"

extern void thrower();

void catcher()
{
	try {

		thrower();

	} catch (Virtual_exception &e) {

		e.func();
		
	}
}
