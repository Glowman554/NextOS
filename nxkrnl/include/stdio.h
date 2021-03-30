#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <mem.h>

#define ZEROPAD		1
#define SIGN		2
#define PLUS		4
#define SPACE		8
#define LEFT		16
#define SMALL		32
#define SPECIAL		64

int sprintf(char *buf, const char *fmt, ...);
int printf_k(const char *fmt, ...);

#endif