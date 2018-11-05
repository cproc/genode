#ifndef STDIO_H
#define STDIO_H

#include <sys/types.h>

#define SEEK_SET 0

struct FILE;
typedef struct FILE FILE;

extern FILE *stderr;

int fclose(FILE *stream);
FILE *fopen(const char *path, const char *mode);
int fprintf(FILE *stream, const char *format, ...);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
void setbuf(FILE *stream, char *buf);
int vfprintf(FILE *stream, const char *format, va_list ap);

#endif
