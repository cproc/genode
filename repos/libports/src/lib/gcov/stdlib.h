#ifndef STDLIB_H
#define STDLIB_H

void abort(void);
void atexit(void (*func)(void));
int atoi(const char *nptr);
void free(void *ptr);
char *getenv(const char *name);
void *malloc(size_t size);

#endif
