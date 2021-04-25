#include <mem.h>

#define BITMAP_SIZE 32768
static uint32_t bitmap[BITMAP_SIZE];

static void pmm_mark_used(void* page);

extern const void kernel_start;
extern const void kernel_end;

void pmm_init(struct multiboot_info* mb_info){
	struct multiboot_mmap* mmap = mb_info->mbs_mmap_addr;
	struct multiboot_mmap* mmap_end = (void*) ((uintptr_t) mb_info->mbs_mmap_addr + mb_info->mbs_mmap_length);

	memset(bitmap, 0, sizeof(bitmap));

	while (mmap < mmap_end) {
		if (mmap->type == 1) {
			uintptr_t addr = mmap->base;
			uintptr_t end_addr = addr + mmap->length;

			while (addr < end_addr) {
				pmm_free((void*) addr);
				addr += 0x1000;
			}
		}
		mmap++;
	}

	uintptr_t addr = (uintptr_t) &kernel_start;
	while (addr < (uintptr_t) &kernel_end) {
		pmm_mark_used((void*) addr);
		addr += 0x1000;
	}
	
	struct multiboot_module* modules = mb_info->mbs_mods_addr;

	pmm_mark_used(mb_info);
	pmm_mark_used(modules);

	int i;
	for (i = 0; i < mb_info->mbs_mods_count; i++) {
		addr = modules[i].mod_start;
		while (addr < modules[i].mod_end) {
			pmm_mark_used((void*) addr);
			addr += 0x1000;
		}
	}
}

void* pmm_alloc(void){
	int i, j;

	for (i = 0; i < BITMAP_SIZE; i++) {
		if (bitmap[i] != 0) {

			for (j = 0; j < 32; j++) {
				if (bitmap[i] & (1 << j)) {
					bitmap[i] &= ~(1 << j);
					debug_write("Allocating memory at 0x%x!", (i * 32 + j) * 4096);
					return (void*)( (i * 32 + j) * 4096);
				}
			}
		}
	}

	return NULL;
}

static void pmm_mark_used(void* page){
	uintptr_t index = (uintptr_t) page / 4096;
	bitmap[index / 32] &= ~(1 << (index % 32));
}

void pmm_free(void* page){
	uintptr_t index = (uintptr_t) page / 4096;
	bitmap[index / 32] |= (1 << (index % 32));
}
