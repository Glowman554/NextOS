#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <driver/serial.h>
#include <config.h>
#include <stdbool.h>
#include <io.h>

#define VIDEO_MEM 0xb8000

#define FOREGROUND_BLACK 0x00
#define FOREGROUND_BLUE 0x01
#define FOREGROUND_GREEN 0x02
#define FOREGROUND_CYAN 0x03
#define FOREGROUND_RED 0x04
#define FOREGROUND_MAGENTA 0x05
#define FOREGROUND_BROWN 0x06
#define FOREGROUND_LIGHTGRAY 0x07
#define FOREGROUND_DARKGRAY 0x08
#define FOREGROUND_LIGHTBLUE 0x09
#define FOREGROUND_LIGHTGREEN 0x0A
#define FOREGROUND_LIGHTCYAN 0x0B
#define FOREGROUND_LIGHTRED 0x0C
#define FOREGROUND_LIGHTMAGENTA 0x0D
#define FOREGROUND_YELLOW 0x0E
#define FOREGROUND_WHITE 0x0F

#define BACKGROUND_BLACK 0x00
#define BACKGROUND_BLUE 0x10
#define BACKGROUND_GREEN 0x20
#define BACKGROUND_CYAN 0x30
#define BACKGROUND_RED 0x40
#define BACKGROUND_MAGENTA 0x50
#define BACKGROUND_BROWN 0x60
#define BACKGROUND_LIGHTGRAY 0x70
#define BACKGROUND_BLINKINGBLACK 0x80
#define BACKGROUND_BLINKINGBLUE 0x90
#define BACKGROUND_BLINKINGGREEN 0xA0
#define BACKGROUND_BLINKINGCYAN 0xB0
#define BACKGROUND_BLINKINGRED 0xC0
#define BACKGROUND_BLINKINGMAGENTA 0xD0
#define BACKGROUND_BLINKINGYELLOW 0xE0
#define BACKGROUND_BLINKINGWHITE 0xF0

extern bool scrole;
extern uint32_t color;

void setx(int i);
void sety(int i);

int getx();
int gety();

void setcolor(uint32_t c);
void kputs(const char *s);
void kputc(char c);
void clrscr();
void kputn(unsigned long x, int base);
void kprintf(const char* fmt, ...);
void setcursor(uint16_t pos);



#endif