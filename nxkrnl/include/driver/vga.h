#ifndef VGA_H
#define VGA_H

#include <io.h>
#include <mem.h>
#include <config.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <driver/serial.h>

#define VGA_BLACK 0x00
#define VGA_BLUE 0x01
#define VGA_GREEN 0x02
#define VGA_CYAN 0x03
#define VGA_RED 0x04
#define VGA_MAGENTA 0x05
#define VGA_BROWN 0x06
#define VGA_LIGHTGRAY 0x07
#define VGA_DARKGRAY 0x08
#define VGA_LIGHTBLUE 0x09
#define VGA_LIGHTGREEN 0x0A
#define VGA_LIGHTCYAN 0x0B
#define VGA_LIGHTRED 0x0C
#define VGA_LIGHTMAGENTA 0x0D
#define VGA_YELLOW 0x0E
#define VGA_WHITE 0x0F

#define MISC_PORT 0x3c2
#define CRTC_INDEX_PORT 0x3d4
#define CRTC_DATA_PORT 0x3d5
#define SEQUENCER_INDEX_PORT 0x3c4
#define SEQUENCER_DATA_PORT 0x3c5
#define GRAPHICS_CONTROLLER_INDEX_PORT 0x3ce
#define GRAPHICS_CONTROLLER_DATA_PORT 0x3cf
#define ATTRIBUTE_CONTROLLER_INDEX_PORT 0x3c0
#define ATTRIBUTE_CONTROLLER_READ_PORT 0x3c1
#define ATTRIBUTE_CONTROLLER_WRITE_PORT 0x3c0
#define ATTRIBUTE_CONTROLLER_RESET_PORT 0x3da

void init_vga();

void setpixel(int x, int y, uint32_t color);
unsigned char getpixel(int x, int y);
volatile void draw_char(char c, int x, int y);
void clear_vga();
void vga_kputc(char c);
void vga_kputs(const char *s);
void set_vga_color(uint32_t fgcolor, uint32_t bgcolor);
bool is_vga_active();
void vga_kputn(unsigned long x, int base);
void vga_kprintf(const char* fmt, ...);


#endif