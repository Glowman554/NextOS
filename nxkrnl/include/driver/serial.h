#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <io.h>

#define PORT 0x3f8   /* COM1 */


void init_serial();
char read_serial();
void write_serial(char a);

#endif