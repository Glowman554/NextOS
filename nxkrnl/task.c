#include <task.h>

void do_nothing(void){
	while(1);
}


struct task task_states[MAX_TASKS];

int current_task = 0;

int proccount = 0;
char nextpid = 1;

bool address_offset_store[MAX_TASKS];

void task_exit(int code){
	debug_write("[%d] Exit Task with error code %d!", task_states[current_task].pid, code);
	if(code == 1){
		kernel_yeet_str("Task returned fail code!");
	}
	if(code == 2) {
			debug_write("[%d] Task killed!", task_states[current_task].pid);
	}

	if(!task_states[current_task].no_free) {
		address_offset_store[task_states[current_task].off /  0x100000 - 2] = false;
	} else {
		debug_write("Not freeing task at 0x%x!", task_states[current_task].off);
	}

	task_states[current_task].active = false;
	pmm_free(task_states[current_task].stack);
	pmm_free(task_states[current_task].user_stack);
	proccount--;

	asm volatile("int $0x20"); // force next task
}

void set_kb_handler(kb_handler handler) {
	task_states[current_task].kb = handler;
}

void kb_handle(char key) {
	for (int i = 0; i < MAX_TASKS; i++) {
		if(task_states[i].active) {
			if(task_states[i].kb != 0) {
				(*(task_states[i].kb))(key);
			}
		}
	}
	
}

void set_mouse_handlers(mouse_move_handler h1, mouse_button_handler h2) {
	task_states[current_task].mmh = h1;
	task_states[current_task].mbh = h2;
}

void mouse_handle_move(long x, long y) {
	for (int i = 0; i < MAX_TASKS; i++) {
		if(task_states[i].active) {
			if(task_states[i].mmh != 0) {
				(*(task_states[i].mmh))(x, y);
			}
		}
	}
}

void mouse_handle_button(int button) {
	for (int i = 0; i < MAX_TASKS; i++) {
		if(task_states[i].active) {
			if(task_states[i].mbh != 0) {
				(*(task_states[i].mbh))(button);
			}
		}
	}
}

struct task* find_task() {
	for (int i = 0; i < MAX_TASKS; i++) {
		if(!task_states[i].active) {
			debug_write("Found empty task at idx %d!", i);
			return &task_states[i];
		}
	}

	// We have no more free task slots
	return NULL;
}

struct task* init_task(void* entry, uint32_t off){
	debug_write("Initializing task at 0x%x!", (uint32_t) entry);
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

	struct task* task = find_task();
	memset(task, 0, sizeof(struct task));
	task->cpu_state = state;
	task->pid = nextpid;
	task->active = true;
	task->stack = stack;
	task->user_stack = user_stack;
	task->off = off;
	nextpid++;
	kprintf("Starting task with pid %d\n", task->pid);
	proccount++;
	return task;
}

uint32_t find_address() {
	for (int i = 0; i < MAX_TASKS; i++) {
		if(!address_offset_store[i]) {
			return i * 0x100000 + 0x200000;
		}
	}
	
	return (uint32_t) NULL;
}

int init_elf(void* image){
	debug_write("Loading elf image at 0x%x!", (uint32_t) image);
	uint32_t addr = find_address();
	debug_write("Using memory location 0x%x!", addr);
	address_offset_store[addr / 0x100000 - 2] = true;
	struct elf_header* header = image;
	struct elf_program_header* ph;
	int i;

	if (header->magic != ELF_MAGIC) {
		kprintf("Found invalid ELF-Magic!\n");
		return 1;
	}

	ph = (struct elf_program_header*) (((char*) image) + header->ph_offset);
	for (i = 0; i < header->ph_entry_count; i++, ph++) {
		void* dest = (void*) ph->virt_addr + addr;
		void* src = ((char*) image) + ph->offset;

		if (ph->type != 1) {
			continue;
		}
		
		memset(dest, 0, ph->mem_size);
		memcpy(dest, src, ph->file_size);
	}
	init_task((void*) header->entry + addr, addr);
	return 0;
}

void init_multitasking(struct multiboot_info* mb_info){
	init_task(do_nothing, (int) NULL);
}


struct cpu_state* schedule(struct cpu_state* cpu){
	for (int i = current_task + 1; i < MAX_TASKS; i++) {
		if(task_states[i].active) {
			current_task = i;
			return task_states[current_task].cpu_state;
		}
	}

	// we have no tasks left or we are at the end of the task list
	// so we start searching from 0 again

	for (int i = 0; i < MAX_TASKS; i++) {
		if(task_states[i].active) {
			current_task = i;
			return task_states[current_task].cpu_state;
		}
	}


	return cpu;
}
