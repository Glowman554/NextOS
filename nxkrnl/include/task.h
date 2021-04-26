#ifndef TASK_H
#define TASK_H
#include <interrupts.h>
#include <stdint.h>
#include <mem.h>
#include <stdbool.h>
#include <elf.h>
#include <console.h>
#include <info_line.h>

#include <stdio.h>
#include <driver/serial.h>

#define MAX_TASKS 32

typedef void (*kb_handler)(char key);
typedef void (*mouse_move_handler)(long x, long y);
typedef void (*mouse_button_handler)(int button);

struct task {
	struct cpu_state*		cpu_state;
	char					pid;
	bool					active;
	bool					destroy;
	void*					stack;
	void*					user_stack;

	kb_handler				kb;
	mouse_move_handler		mmh;
	mouse_button_handler	mbh;
};

extern int proccount;
extern struct task task_states[MAX_TASKS];

extern int current_task;

struct cpu_state* schedule(struct cpu_state* cpu);
void init_multitasking(struct multiboot_info* mb_info);
struct task* init_task(void* entry);
int init_elf(void* image);
void task_exit(int code);

void set_kb_handler(kb_handler handler);
void set_mouse_handlers(mouse_move_handler h1, mouse_button_handler h2);

void kb_handle(char key);
void mouse_handle_move(long x, long y);
void mouse_handle_button(int button);


#endif