#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include <console.h>
#include <mem.h>

#define isdigit(c) (c >= '0' && c <= '9')

#define isspace(c) (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ')

#define __do_div(n, base) ({					\
int __res;										\
__res = ((unsigned long) n) % (unsigned) base;	\
n = ((unsigned long) n) / (unsigned) base;		\
__res; })

void* memset(void* buf, int c, int n);
void* memcpy(void* dest, const void* src, size_t n);
int strcmp(char *str1, char *str2);

int strlen(char *src);
char *strcpy(char *dest, const char *src);

size_t strnlen(const char *s, size_t maxlen);
char* strchr(const char* s, int c);

double strtod(const char* string, char** endPtr);

#endif