#ifndef STDIO_H
#define STDIO_H

#define SEEK_SET 0

struct FILE;
typedef struct FILE FILE;

extern FILE *stderr;

int fprintf(FILE *stream, const char *format, ...);

#endif
