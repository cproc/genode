#include <base/env.h>
#include <base/heap.h>
#include <base/log.h>
#include <base/printf.h>
#include <file_system_session/connection.h>
#include <file_system/util.h>
#include <util/string.h>

extern "C" {
#include "stdio.h"
}

using Genode::size_t;
typedef Genode::Path<File_system::MAX_PATH_LEN> Absolute_path;

struct FILE { };

static FILE stderr_file;

FILE *stderr = &stderr_file;
extern "C" void wait_for_continue();
struct Gcov_env
{
	Genode::Env             &env;
	Genode::Heap             heap { env.ram(), env.rm() };
	Genode::Allocator_avl    fs_alloc { &heap };
	File_system::Connection  fs { env, fs_alloc, "gcov" };

	Gcov_env(Genode::Env &env) : env(env) { wait_for_continue(); }
	~Gcov_env() { Genode::log("~Gcov_env()"); }
};

static Genode::Constructible<Gcov_env> gcov_env;

void gcov_init(Genode::Env &env)
{
	Genode::log("*** gcov_init()");
	gcov_env.construct(env);
}

extern "C" void *abort()
{
	Genode::log("*** abort(): not implemented");
	return 0;
}

extern void genode_atexit(void (*func)(void));                                                                                                                                                                                               
extern "C" void atexit(void (*func)(void))
{
	Genode::log("*** atexit()");
    genode_atexit(func);
}

extern "C" void *atoi()
{
	Genode::log("*** atoi(): not implemented");
	return 0;
}

extern "C" void *fclose()
{
	Genode::log("*** fclose(): not implemented");
	return 0;
}

extern "C" FILE *fopen(const char *path, const char *mode)
{
	Genode::log("*** fopen(\"", Genode::Cstring(path), "\", \"", Genode::Cstring(mode), "\")");

	// XXX: handle r+b
	if (Genode::strcmp(mode, "w+b", 3) != 0)
		return 0;
	
	Absolute_path dir_path(path);
	dir_path.strip_last_element();

	Absolute_path file_name(path);
	file_name.keep_only_last_element();

	try {
		File_system::Dir_handle dir = gcov_env->fs.dir(dir_path.base(), false);
	} catch (File_system::Lookup_failed) {
		return 0;
	}


}

int fprintf(FILE *stream, const char *format, ...)
{
	//Genode::log("fprintf(): ", Genode::Cstring(format));
	Genode::log("fprintf()");

	if (stream != stderr)
		return 0;

	va_list list;
	va_start(list, format);

	Genode::vprintf(format, list);
	va_end(list);

	return 0;
}

extern "C" void *fread()
{
	Genode::log("*** fread(): not implemented");
	return 0;
}

extern "C" void free(void *ptr)
{
	Genode::log("*** free()");
	gcov_env->heap.free(ptr, 0);
}

extern "C" void *fseek()
{
	Genode::log("*** fseek(): not implemented");
	return 0;
}

extern "C" void *ftell()
{
	Genode::log("*** fread(): not implemented");
	return 0;
}

extern "C" size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	Genode::log("*** fwrite(", ptr, ", ", size, ", ", nmemb, ", ", stream, "): not implemented");
	return 0;
}

extern "C" char *getenv(const char *name)
{
	Genode::log("*** getenv(\"", Genode::Cstring(name), "\"): not implemented");
	return nullptr;
}

extern "C" void *malloc(size_t size)
{
	Genode::log("*** malloc(", size, ")");
	
	void *res = nullptr;
	gcov_env->heap.alloc(size, &res);
	return res;
}

extern "C" void *setbuf()
{
	Genode::log("*** setbuf(): not implemented");
	return 0;
}

extern "C" char *strcpy(char *dest, const char *src)
{
	Genode::log("*** strcpy(\"", Genode::Cstring(src), "\")");
	return Genode::strncpy(dest, src, Genode::strlen(src) + 1);
}

extern "C" size_t strlen(const char *s)
{
	Genode::log("*** strlen(\"", Genode::Cstring(s), "\")");
	return Genode::strlen(s);
}

extern "C" int vfprintf(FILE *stream, const char *format, va_list ap)
{
	if (stream != stderr)
		return 0;

	Genode::vprintf(format, ap);
	return 0;
}
