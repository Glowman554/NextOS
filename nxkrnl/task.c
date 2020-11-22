#include <task.h>

static void task_a(void){
	while (1) {
		kprintf("A");
	}
}

static void task_b(void){
	while (1) {
		kprintf("B");
	}
}

static void task_c(void){
	while (1) {
		kprintf("C");
	}
}

static void task_d(void){
	while (1) {
		kprintf("D");
	}
}

struct cpu_state* init_task(void* entry){
	uint8_t* stack = pmm_alloc();
	uint8_t* user_stack = pmm_alloc();


	struct cpu_state new_state = {
		.eax = 0,
		.ebx = 0,
		.ecx = 0,
		.edx = 0,
		.esi = 0,
		.edi = 0,
		.ebp = 0,
		.esp = (uint32_t) user_stack + 4096,
		.eip = (uint32_t) entry,

		.cs  = 0x18 | 0x03,
		.ss  = 0x20 | 0x03,

		.eflags = 0x200,
	};

	struct cpu_state* state = (void*) (stack + 4096 - sizeof(new_state));
	*state = new_state;

	return state;
}

static int current_task = -1;
static int num_tasks = 4;
static struct cpu_state* task_states[4];

void init_multitasking(void){
	task_states[0] = init_task(task_a);
	task_states[1] = init_task(task_b);
	task_states[2] = init_task(task_c);
	task_states[3] = init_task(task_d);
}

struct cpu_state* schedule(struct cpu_state* cpu){
	if (current_task >= 0) {
		task_states[current_task] = cpu;
	}
	current_task++;
	current_task %= num_tasks;
	
	cpu = task_states[current_task];

	return cpu;
}
