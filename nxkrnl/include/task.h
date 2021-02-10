#ifndef TASK_H
#define TASK_H
#include <interrupts.h>
#include <stdint.h>
#include <mem.h>
#include <elf.h>
#include <console.h>

typedef void (*kb_handler)(char key);

struct task {
	struct cpu_state*	cpu_state;
	struct task*		next;
	//Fix for module load error some free space
	uint64_t 			r1;
	uint64_t 			r2;
	uint64_t 			r3;
	uint64_t 			r4;
	uint64_t 			r5;
	uint64_t 			r6;
	uint64_t 			r7;
	uint64_t 			r8;
	uint64_t 			r9;
	uint64_t 			r10;
	uint64_t 			r11;
	uint64_t 			r12;
	uint64_t 			r13;
	uint64_t 			r14;
	uint64_t 			r15;
	uint64_t 			r16;
	uint64_t 			r17;
	uint64_t 			r18;
	uint64_t 			r19;
	uint64_t 			r20;
	char				pid;
};

struct cpu_state* schedule(struct cpu_state* cpu);
void init_multitasking(struct multiboot_info* mb_info);
struct task* init_task(void* entry);
int init_elf(void* image);
void task_exit(int code);

void kb_handle(char key);
void set_kb_handler(kb_handler handler);


#endif