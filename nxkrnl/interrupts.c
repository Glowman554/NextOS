#include <interrupts.h>
#include <task.h>
#include <gdt.h>
#include <syscall.h>
#define IDT_FLAG_INTERRUPT_GATE 0xe
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0 0x00
#define IDT_FLAG_RING3 0x60

void do_handle_interrupt(uint32_t interrupt_num); 

extern void intr_stub_0(void);
extern void intr_stub_1(void);
extern void intr_stub_2(void);
extern void intr_stub_3(void);
extern void intr_stub_4(void);
extern void intr_stub_5(void);
extern void intr_stub_6(void);
extern void intr_stub_7(void);
extern void intr_stub_8(void);
extern void intr_stub_9(void);
extern void intr_stub_10(void);
extern void intr_stub_11(void);
extern void intr_stub_12(void);
extern void intr_stub_13(void);
extern void intr_stub_14(void);
extern void intr_stub_15(void);
extern void intr_stub_16(void);
extern void intr_stub_17(void);
extern void intr_stub_18(void);

extern void intr_stub_32(void);
extern void intr_stub_33(void);
extern void intr_stub_34(void);
extern void intr_stub_35(void);
extern void intr_stub_36(void);
extern void intr_stub_37(void);
extern void intr_stub_38(void);
extern void intr_stub_39(void);
extern void intr_stub_40(void);
extern void intr_stub_41(void);
extern void intr_stub_42(void);
extern void intr_stub_43(void);
extern void intr_stub_44(void);
extern void intr_stub_45(void);
extern void intr_stub_46(void);
extern void intr_stub_47(void);

extern void intr_stub_48(void);

static long long unsigned int idt[IDT_ENTRIES];

uint32_t timer_tick = 0;

static void idt_set_entry(int i, void (*fn)(), unsigned int selector, int flags){
	char buffer[1000];
	sprintf(buffer, "Setting idt entry at %d with handler 0x%x and selector 0x%x and the flags 0x%x", i, (uint32_t) fn, selector, flags);
	debug_write(buffer);

	unsigned long int handler = (unsigned long int) fn;
	idt[i] = handler & 0xffffLL;
	idt[i] |= (selector & 0xffffLL) << 16;
	idt[i] |= (flags & 0xffLL) << 40;
	idt[i] |= ((handler>> 16) & 0xffffLL) << 48;
}

void init_pic(){
	outb(0x20, 0x11);
	outb(0x21, 0x20);
	outb(0x21, 0x04);
	outb(0x21, 0x01);
	
	outb(0xa0, 0x11);
	outb(0xa1, 0x28);
	outb(0xa1, 0x02);
	outb(0xa1, 0x01);
	
	outb(0x21, 0x0);
	outb(0xa1, 0x0);
}

void init_intr(){
	struct {
		unsigned short int limit;
		void* pointer;
	} __attribute__((packed)) idtp = {
		.limit = IDT_ENTRIES * 8 - 1,
		.pointer = idt,
	};
	init_pic();
	idt_set_entry(0, intr_stub_0, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(1, intr_stub_1, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(2, intr_stub_2, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(3, intr_stub_3, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(4, intr_stub_4, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(5, intr_stub_5, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(6, intr_stub_6, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(7, intr_stub_7, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(8, intr_stub_8, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(9, intr_stub_9, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(10, intr_stub_10, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(11, intr_stub_11, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(12, intr_stub_12, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(13, intr_stub_13, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(14, intr_stub_14, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(15, intr_stub_15, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(16, intr_stub_16, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(17, intr_stub_17, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(18, intr_stub_18, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);

	// IRQ-Handler
	idt_set_entry(32, intr_stub_32, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(33, intr_stub_33, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(34, intr_stub_34, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(35, intr_stub_35, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(36, intr_stub_36, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(37, intr_stub_37, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(38, intr_stub_38, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(39, intr_stub_39, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(40, intr_stub_40, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(41, intr_stub_41, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(42, intr_stub_42, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(43, intr_stub_43, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(44, intr_stub_44, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(45, intr_stub_45, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(46, intr_stub_46, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_entry(47, intr_stub_47, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);

	// Syscall
	idt_set_entry(48, intr_stub_48, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING3 | IDT_FLAG_PRESENT);
	asm volatile("lidt %0" : : "m" (idtp));
	debug_write("Enabling interrupts!");
	asm volatile("sti");
	
}

void reset_timer_tick(){
	timer_tick = 0;
}

uint32_t get_timer_tick(){
	return timer_tick;
}

struct cpu_state* handle_interrupt(struct cpu_state* cpu){
	struct cpu_state* new_cpu = cpu;
	if(cpu->intr <= 0x1f){
		print_exception(cpu->intr);
        extern void dump_kernel_panic(struct cpu_state* cpu);
        dump_kernel_panic(cpu);
		if(is_vga_active()){
			
			vga_kprintf("eax: 0x%x, ebx: 0x%x\necx: 0x%x, edx: 0x%x\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
			vga_kprintf("esi: 0x%x, edi: 0x%x\nebp: 0x%x\n", cpu->esi, cpu->edi, cpu->ebp);
			vga_kprintf("intr: 0x%x, error: 0x%x\n", cpu->intr, cpu->error);
			vga_kprintf("eip: 0x%x, cs 0x%x\neflags: 0x%x\n", cpu->eip, cpu->cs, cpu->eflags);
			vga_kprintf("esp: 0x%x, ss: 0x%x\n", cpu->esp, cpu->ss);
			
			set_vga_color(VGA_YELLOW, VGA_BLUE);
			//vga_kprintf("\nAbboniert Vaspel auf YouTube\n");
			//vga_kprintf("https://www.youtube.com/c/Vaspelderechte\n");
		} else {
						
			kprintf("\neax: 0x%x, ebx: 0x%x, ecx: 0x%x, edx: 0x%x\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
			kprintf("esi: 0x%x, edi: 0x%x, ebp: 0x%x\n", cpu->esi, cpu->edi, cpu->ebp);
			kprintf("intr: 0x%x, error: 0x%x\n", cpu->intr, cpu->error);
			kprintf("eip: 0x%x, cs 0x%x, eflags: 0x%x\n", cpu->eip, cpu->cs, cpu->eflags);
			kprintf("esp: 0x%x, ss: 0x%x\n", cpu->esp, cpu->ss);
			
			setcolor(BACKGROUND_BLACK | FOREGROUND_YELLOW);
			//kprintf("\nEin Abschliesender Tipp: Abboniert Vaspel auf YouTube\n");
			//kprintf("https://www.youtube.com/c/Vaspelderechte\n");

		}
		
		while(1){
			asm volatile("cli; hlt");
		}
	}
	if (cpu->intr >= 0x20 && cpu->intr <= 0x2f) {

		extern bool is_init_done;
		
		if(cpu->intr == 0x20 && is_init_done){
			new_cpu = schedule(cpu);
			set_tss(1, (uint32_t) (new_cpu + 1));
			timer_tick++;
			drawinfo();
		}
		
		do_handle_interrupt(cpu->intr);
		
		if (cpu->intr >= 0x28) {
			outb(0xa0, 0x20);
		}
		outb(0x20, 0x20);
	} else if(cpu->intr == 0x30){
		new_cpu = syscall(cpu);
	}
	return new_cpu;
}
