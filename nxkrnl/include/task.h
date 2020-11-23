#ifndef TASK_H
#define TASK_H
#include <interrupts.h>
#include <stdint.h>
#include <mem.h>
#include <elf.h>
#include <console.h>

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
	char				pid;
};

struct cpu_state* schedule(struct cpu_state* cpu);
void init_multitasking(struct multiboot_info* mb_info);
struct task* init_task(void* entry);
int init_elf(void* image);
void task_exit(int code);


#endif