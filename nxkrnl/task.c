#include <task.h>

void do_nothing(void){
	while(1);
}


static struct task task_states[512];

int current_task = 0;

kb_handler kb_intr_handler = NULL;
mouse_move_handler mouse_intr_move_handler = NULL;
mouse_button_handler mouse_intr_button_handler = NULL;

int proccount = 0;
char nextpid = 1;

void task_exit(int code){
	debug_write("[%d] Exit Task with error code %d!", task_states[current_task].pid, code);
	if(code == 1){
		kernel_yeet(task_states[current_task].cpu_state);
	}
	task_states[current_task].active = false;
	pmm_free(task_states[current_task].stack);
	pmm_free(task_states[current_task].user_stack);
	proccount--;
}

void set_kb_handler(kb_handler handler) {
	kb_intr_handler = handler;
}

void kb_handle(char key) {
	if(kb_intr_handler != 0)
		(*(kb_intr_handler))(key);
}

void set_mouse_handlers(mouse_move_handler h1, mouse_button_handler h2) {
	mouse_intr_move_handler = h1;
	mouse_intr_button_handler = h2;
}

void mouse_handle_move(long x, long y) {
	if(mouse_intr_move_handler != 0)
		(*(mouse_intr_move_handler))(x, y);
}

void mouse_handle_button(int button) {
	if(mouse_intr_button_handler != 0)
		(*(mouse_intr_button_handler))(button);
}

struct task* find_task() {
	for (int i = 0; i < 512; i++) {
		if(!task_states[i].active) {
			debug_write("Found empty task ad idx %d!", i);
			return &task_states[i];
		}
	}

	// We have no more free task slots
	return NULL;
}

struct task* init_task(void* entry){
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
	task->cpu_state = state;
	task->pid = nextpid;
	task->active = true;
	task->stack = stack;
	task->user_stack = user_stack;
	nextpid++;
	kprintf("Starting task with pid %d\n", task->pid);
	proccount++;
	return task;
}

int init_elf(void* image){
	debug_write("Loading elf image at 0x%x!", (uint32_t) image);
	struct elf_header* header = image;
	struct elf_program_header* ph;
	int i;

	if (header->magic != ELF_MAGIC) {
		kprintf("Found invalid ELF-Magic!\n");
		return 1;
	}

	ph = (struct elf_program_header*) (((char*) image) + header->ph_offset);
	for (i = 0; i < header->ph_entry_count; i++, ph++) {
		void* dest = (void*) ph->virt_addr;
		void* src = ((char*) image) + ph->offset;

		if (ph->type != 1) {
			continue;
		}
		
		memset(dest, 0, ph->mem_size);
		memcpy(dest, src, ph->file_size);
	}
	init_task((void*) header->entry);
	return 0;
}

void init_multitasking(struct multiboot_info* mb_info){
	init_task(do_nothing);
}


struct cpu_state* schedule(struct cpu_state* cpu){
	for (int i = current_task + 1; i < 512; i++) {
		if(task_states[i].active) {
			current_task = i;
			return task_states[current_task].cpu_state;
		}
	}

	// we have no tasks left or we are at the end of the task list
	// so we start searching from 0 again

	for (int i = 0; i < 512; i++) {
		if(task_states[i].active) {
			current_task = i;
			return task_states[current_task].cpu_state;
		}
	}


	return cpu;
}
