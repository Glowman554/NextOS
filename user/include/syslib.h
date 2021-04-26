#ifndef SYSLIB_H
#define SYSLIB_H

#include <stdarg.h>
#include <stdint.h>
#include <multiboot.h>
#include <fs.h>
#include <stdbool.h>

#define SYSCALL_PUTC				0
#define SYSCALL_PUTS				1
#define SYSCALL_PUTN				2
#define SYSCALL_CLRSCR				3
#define SYSCALL_SETCOLOR			4
#define SYSCALL_KVERSION			5
#define SYSCALL_KVENDOR				6
#define SYSCALL_EXEC				7
#define SYSCALL_INIT_TASK			8
#define SYSCALL_TASK_EXIT			9
#define SYSCALL_REBOOT				10
#define SYSCALL_GET_TICK			12
#define SYSCALL_RESET_TICK			13
#define SYSCALL_MULTIBOOT			14
#define SYSCALL_VGA_MODE			15
#define SYSCALL_VGA_SETPIXEL		16
#define SYSCALL_VGA_SETCOLOR		17
#define SYSCALL_LOAD_INITRD			18
#define SYSCALL_FSROOT_READDIR		19
#define SYSCALL_FSROOT_FINDDIR		20
#define SYSCAlL_FSROOT_READ			21
#define SYSCALL_GET_BUFFER			22
#define SYSCALL_INIT_DRIVER		 23
#define SYSCALL_CALL_DRIVER		 24
#define SYSCALL_FIND_DRIVER		 25
#define SYSCALL_DRAW_CHAR		   26
#define SYSCALL_SETX				27
#define SYSCALL_SETY				28
#define SYSCALL_GETX				29
#define SYSCALL_GETY				30
#define SYSCALL_SET_KB_HANDLER	  31
#define SYSCALL_SET_MOUSE_HANDLER   32
#define SYSCALL_GETPIXEL			33

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

typedef int (*driver_handler_ptr)(void*);
typedef void (*kb_handler)(char key);
typedef void (*mouse_move_handler)(long x, long y);
typedef void (*mouse_button_handler)(int button);
typedef void (*mouse_handler)(int button, int x, int y);

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
void reset_timer_tick();
uint32_t get_timer_tick();
struct multiboot_info* get_mb_ptr();
int strcmp(char *str1, char *str2);
void init_vga();
void setpixel(int x, int y, uint32_t color);
void set_vga_color(uint32_t fgcolor, uint32_t bgcolor);
void load_initrd(char* file);
struct dirent* fsroot_readdir(int index);
fs_node_t* fsroot_finddir(char* file);
void fsroot_read(fs_node_t *fsnode, uint32_t offset, uint32_t size);
uint8_t* get_buffer();
int init_driver(char* name, driver_handler_ptr driver_handler);
int call_driver_handler(int id, void* data);
int find_driver_by_name(char* name);
void draw_char(char c, int x, int y);
void setx(int i);
void sety(int i);
int getx();
int gety();
char* get_input();
void set_kb_handler(kb_handler handler);
void set_mouse_handlers(mouse_move_handler h1, mouse_button_handler h2);
char getpixel(int x, int y);
int strlen(char *src);
char getchar();
void set_mouse_handler(mouse_handler h);
void claim_kb_handler();
void claim_mouse_handlers();

#endif