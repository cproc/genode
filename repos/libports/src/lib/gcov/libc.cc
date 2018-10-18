#include <base/env.h>
#include <base/heap.h>
#include <base/log.h>
#include <base/printf.h>
#include <base/sleep.h>
#include <file_system_session/connection.h>
#include <file_system/util.h>
#include <util/string.h>

extern "C" {
#include "stdio.h"
}

typedef Genode::Path<File_system::MAX_PATH_LEN> Absolute_path;

struct FILE { };

static FILE stderr_file;

FILE *stderr = &stderr_file;

struct Gcov_env
{
	Genode::Env             &env;
	Genode::Heap             heap { env.ram(), env.rm() };
	Genode::Allocator_avl    fs_alloc { &heap };
	File_system::Connection  fs { env, fs_alloc, "gcov" };
	unsigned long            seek_offset { 0 };

	/* only one file is open at a time */
	Genode::Constructible<File_system::File_handle> file_handle;
	FILE                                            file;

	Gcov_env(Genode::Env &env) : env(env) { }
};


static Genode::Constructible<Gcov_env> gcov_env;


void gcov_init(Genode::Env &env)
{
	Genode::log("*** gcov_init()");
	gcov_env.construct(env);
}


extern "C" void abort()
{
	Genode::error("*** abort(): not implemented");
	Genode::sleep_forever();
}


extern void genode_atexit(void (*func)(void));                                                                                                                                                                                               
extern "C" void atexit(void (*func)(void))
{
	Genode::log("*** atexit()");
    genode_atexit(func);
}


extern "C" int atoi(const char *nptr)
{
	Genode::error("*** atoi(): not implemented");
	return 0;
}


extern "C" int fclose(FILE *stream)
{
	Genode::log("*** fclose()");
	gcov_env->fs.close(*(gcov_env->file_handle));
	gcov_env->file_handle.destruct();
	return 0;
}


extern "C" FILE *fopen(const char *path, const char *mode)
{
	Genode::log("*** fopen(\"", Genode::Cstring(path), "\", \"", Genode::Cstring(mode), "\")");

	Absolute_path dir_path(path);
	dir_path.strip_last_element();

	Absolute_path file_name(path);
	file_name.keep_only_last_element();

	File_system::Dir_handle dir = File_system::ensure_dir(gcov_env->fs, dir_path.base());

	try {
		gcov_env->file_handle.construct(gcov_env->fs.file(dir,
		                                                  file_name.base() + 1,
		                                                  File_system::READ_WRITE,
		                                                  false));
		if (Genode::strcmp(mode, "w+b", 3) == 0)
			gcov_env->fs.truncate(*(gcov_env->file_handle), 0);

	} catch (File_system::Lookup_failed) {

		if (Genode::strcmp(mode, "w+b", 3) == 0)
			gcov_env->file_handle.construct(gcov_env->fs.file(dir,
			                                                  file_name.base() + 1,
			                                                  File_system::READ_WRITE,
		    	                                              true));
		else
			return nullptr;
	}

	gcov_env->seek_offset = 0;

	return &gcov_env->file;
}


extern "C" int fprintf(FILE *stream, const char *format, ...)
{
	if (stream != stderr) {
		Genode::error("fprintf(): not implemented");
		return 0;
	}

	va_list list;
	va_start(list, format);

	Genode::vprintf(format, list);
	va_end(list);

	return 0;
}


extern "C" size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	Genode::log("*** fread(", size, ", ", nmemb, ")");

	size_t bytes_read = File_system::read(gcov_env->fs, *(gcov_env->file_handle),
	                                      ptr, size * nmemb,
	                                      gcov_env->seek_offset);

	gcov_env->seek_offset += bytes_read;
	
	if (bytes_read == (size * nmemb))
		return nmemb;
	
	return 0;
}


extern "C" void free(void *ptr)
{
	Genode::log("*** free()");
	gcov_env->heap.free(ptr, 0);
}


extern "C" int fseek(FILE *stream, long offset, int whence)
{
	if (whence != 0)
		Genode::error("fseek(): unsupported 'whence'");

	gcov_env->seek_offset = offset;

	return 0;
}


extern "C" long ftell(FILE *stream)
{
	Genode::error("*** ftell()");
	return gcov_env->seek_offset;
}


extern "C" size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	if (stream == stderr) {
		Genode::printf((const char*)ptr);
		return 0;
	}

	Genode::log("*** fwrite(", size, ", ", nmemb, ")");

	size_t bytes_written = File_system::write(gcov_env->fs,
	                                          *(gcov_env->file_handle),
	                                          ptr, size * nmemb,
	                                          gcov_env->seek_offset);

	gcov_env->seek_offset += bytes_written;

	if (bytes_written == (size * nmemb))
		return nmemb;

	return 0;
}


extern "C" char *getenv(const char *name)
{
	Genode::error("*** getenv(\"", Genode::Cstring(name), "\"): not implemented");
	return nullptr;
}


extern "C" void *malloc(size_t size)
{
	Genode::log("*** malloc(", size, ")");
	
	void *res = nullptr;
	gcov_env->heap.alloc(size, &res);
	return res;
}


extern "C" void setbuf(FILE *stream, char *buf)
{
	if (buf)
		Genode::error("*** setbuf(", (void*)buf, "): not implemented");
	else
		Genode::log("*** setbuf(", (void*)buf, ")");
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
