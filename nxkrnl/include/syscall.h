#ifndef SYSCALL_H
#define SYSCALL_H

#include <interrupts.h>
#include <console.h>
#include <config.h>
#include <exec.h>
#include <task.h>
#include <system.h>
#include <driver/vga.h>
#include <stdint.h>
#include <stdbool.h>
#include <fs/fs.h>
#include <fs/initrd.h>
#include <externel_driver.h>

#define SYSCALL_PUTC			0
#define SYSCALL_PUTS			1
#define SYSCALL_PUTN			2
#define SYSCALL_CLRSCR			3
#define SYSCALL_SETCOLOR		4
#define SYSCALL_KVERSION		5
#define SYSCALL_KVENDOR			6
#define SYSCALL_EXEC			7
#define SYSCALL_INIT_TASK		8
#define SYSCALL_TASK_EXIT		9
#define SYSCALL_REBOOT			10
#define SYSCALL_GETCHAR			11
#define SYSCALL_GET_TICK		12
#define SYSCALL_RESET_TICK		13
#define SYSCALL_MULTIBOOT		14
#define SYSCALL_VGA_MODE		15
#define SYSCALL_VGA_SETPIXEL	16
#define SYSCALL_VGA_SETCOLOR	17
#define SYSCALL_LOAD_INITRD		18
#define SYSCALL_INITRD_READDIR	19
#define SYSCALL_INITRD_FINDDIR	20
#define SYSCAlL_INITRD_READ		21
#define SYSCALL_GET_BUFFER		22
#define SYSCALL_INIT_DRIVER     23
#define SYSCALL_CALL_DRIVER     24
#define SYSCALL_FIND_DRIVER     25
#define SYSCALL_DRAW_CHAR       26

struct cpu_state* syscall(struct cpu_state* cpu);

#endif