#include <base/log.h>

int stderr = 2;

extern "C" void *abort()
{
	Genode::log("*** abort()");
	return 0;
}

extern "C" void *atexit()
{
	Genode::log("*** atexit()");
	return 0;
}

extern "C" void *atoi()
{
	Genode::log("*** atoi()");
	return 0;
}

extern "C" void *fclose()
{
	Genode::log("*** fclose()");
	return 0;
}

extern "C" void *fopen()
{
	Genode::log("*** fopen()");
	return 0;
}

extern "C" void *fprintf()
{
	Genode::log("*** fprintf()");
	return 0;
}

extern "C" void *fread()
{
	Genode::log("*** fread()");
	return 0;
}

extern "C" void *free()
{
	Genode::log("*** free()");
	return 0;
}

extern "C" void *fseek()
{
	Genode::log("*** fseek()");
	return 0;
}

extern "C" void *ftell()
{
	Genode::log("*** fread()");
	return 0;
}

extern "C" void *fwrite()
{
	Genode::log("*** fwrite()");
	return 0;
}

extern "C" void *getenv()
{
	Genode::log("*** getenv()");
	return 0;
}

extern "C" void *malloc()
{
	Genode::log("*** malloc()");
	return 0;
}

extern "C" void *setbuf()
{
	Genode::log("*** setbuf()");
	return 0;
}

extern "C" void *strcpy()
{
	Genode::log("*** strcpy()");
	return 0;
}

extern "C" void *strlen()
{
	Genode::log("*** strlen()");
	return 0;
}

extern "C" void *vfprintf()
{
	Genode::log("*** vfprintf()");
	return 0;
}
