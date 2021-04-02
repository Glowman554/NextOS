#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <io.h>
#include <stdarg.h>
#include <parser.h>

#define PORT 0x3f8   /* COM1 */


void init_serial();
char read_serial();
void write_serial(char a);

void kputs_serial(const char *s);
void kputc_serial(char c);
void kputn_serial(unsigned long x, int base);
void kprintf_serial(const char* fmt, ...);

#define debug_write(what) 	if(global_kernel_info.debug) kprintf_serial("[%s, %d] %s\n", __FILE__, __LINE__, what)

#endif