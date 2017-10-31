#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __np_size_t size_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
int strcmp(const char* s1, const char* s2);
char *strchr(const char *s, int c);
size_t strspn(const char *s1, const char *s2);
size_t strcspn(const char *s1, const char *s2);
char *strtok(char * str, const char * delim);

#ifdef _NC_RESTRICT
char *strcpy(char *restrict dest, const char *restrict src);
#else
char *strcpy(char *dest, const char* src);
#endif


#ifdef __cplusplus
}
#endif

#endif
