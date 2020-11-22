#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void* memset(void* buf, int c, int n);
void* memcpy(void* dest, const void* src, size_t n);
int strcmp(char *str1, char *str2);

int strlen(char *src);


#endif