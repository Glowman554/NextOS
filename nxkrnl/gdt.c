#include <gdt.h>
static uint64_t gdt[GDT_ENTRIES];
static uint32_t tss[32] = { 0, 0, 0x10 };


static void gdt_set_entry(int i, unsigned int base, unsigned int limit, int flags){
	gdt[i] = limit & 0xffffLL;
	gdt[i] |= (base & 0xffffffLL) << 16;
	gdt[i] |= (flags & 0xffLL) << 40;
	gdt[i] |= ((limit >> 16) & 0xfLL) << 48;
	gdt[i] |= ((flags >> 8 )& 0xffLL) << 52;
	gdt[i] |= ((base >> 24) & 0xffLL) << 56;
}

void init_gdt(){
	struct {
		uint16_t limit;
		void* pointer;
	} __attribute__((packed)) gdtp = {
		.limit = GDT_ENTRIES * 8 - 1,
		.pointer = gdt,
	};
	
	gdt_set_entry(0, 0, 0, 0);
	gdt_set_entry(1, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_CODESEG | GDT_FLAG_4K | GDT_FLAG_PRESENT);
	gdt_set_entry(2, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_DATASEG | GDT_FLAG_4K | GDT_FLAG_PRESENT);
	gdt_set_entry(3, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_CODESEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
	gdt_set_entry(4, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_DATASEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
	
	asm volatile("lgdt %0" : : "m" (gdtp));
	asm volatile(
		"mov $0x10, %ax;"
		"mov %ax, %ds;"
		"mov %ax, %es;"
		"mov %ax, %ss;"
		"ljmp $0x8, $.1;"
		".1:"
	);
}