#ifndef SYSLIB_H
#define SYSLIB_H

#include <stdarg.h>
#include <stdint.h>

#define SYSCALL_PUTC		0
#define SYSCALL_PUTS		1
#define SYSCALL_PUTN		2
#define SYSCALL_CLRSCR		3
#define SYSCALL_SETCOLOR	4
#define SYSCALL_KVERSION	5
#define SYSCALL_KVENDOR		6
#define SYSCALL_EXEC		7
#define SYSCALL_INIT_TASK	8
#define SYSCALL_TASK_EXIT	9
#define SYSCALL_REBOOT		10
#define SYSCALL_GETCHAR		11
#define SYSCALL_GET_TICK	12
#define SYSCALL_RESET_TICK	13


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

void kputc(char c);
void kputs(const char *s);
void kputn(int n, int base);
void clrscr();
void setcolor(uint32_t color);
int kversion();
char* kvendor();
void exec(char* file);
void kprintf(const char* fmt, ...);
void init_task(void* entry);
void task_exit(int code);
void reboot();
char getchar();
void reset_timer_tick();
uint32_t get_timer_tick();
int strcmp(char *str1, char *str2);

#endif