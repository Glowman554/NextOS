#include <task.h>

void do_nothing(void){
	while(1);
}

static struct task* first_task = NULL;
static struct task* current_task = NULL;

kb_handler kb_intr_handler = NULL;
mouse_move_handler mouse_intr_move_handler = NULL;
mouse_button_handler mouse_intr_button_handler = NULL;

int proccount = 0;
char nextpid = 1;

void task_exit(int code){
	char buffer[100];
	sprintf(buffer, "[%d] Exit Task with error code %d!", current_task->pid, code);
	debug_write(buffer);
	if(code == 1){
		asm("int $0x1");
	}
	proccount--;
	struct task* temp = first_task;
	if(current_task->pid == first_task->pid){
		first_task = temp->next;
		return;
	}
	
	while(temp->next != NULL){
		if(temp->next->pid == current_task->pid){
			current_task = temp->next;
		}
		temp = temp->next;
	}
	
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

struct task* init_task(void* entry){
	char buffer[100];
	sprintf(buffer, "Initializing task at 0x%x!", (uint32_t) entry);
	debug_write(buffer);
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

	struct task* task = pmm_alloc();
	task->cpu_state = state;
	task->next = first_task;
	task->pid = nextpid;
	nextpid++;
	kprintf("Starting task with pid %d\n", task->pid);
	proccount++;
	first_task = task;
	return task;
}

int init_elf(void* image){
	char buffer[100];
	sprintf(buffer, "Loading elf image at 0x%x!", (uint32_t) image);
	debug_write(buffer);
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
	if (current_task != NULL) {
		current_task->cpu_state = cpu;
	}
	if(current_task == NULL){
		current_task = first_task;
	} else {
		current_task = current_task->next;
		if(current_task == NULL){
			current_task = first_task;
		}
	}
	
	cpu = current_task->cpu_state;

	return cpu;
}
