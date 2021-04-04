#ifndef NEXTFS_H
#define NEXTFS_H

extern "C" {
	#include <mem.h>
}

#include <stdint.h>
#include <driver/ata.h>

#define MAGIC 0xf00dbabe

struct nextfs_header {
	uint64_t magic;
	char name[64];
	char origin[16];

	uint16_t current_sector;
	uint16_t file_header_index;
} __attribute__((packed));

struct nextfs_file_header {
	char name[64];
	uint16_t start_sector;
	uint16_t length;
};

class NextFS {
	private:
		nextfs_file_header* file_array;
		nextfs_header* header;

		AdvancedTechnologyAttachment* ata;
	
	public:
		NextFS(AdvancedTechnologyAttachment* ata);
		~NextFS();

		bool is_next_fs();

		void print_fs_info();

		void format(char* name);
		void new_text_file(char* name, char* data);
		void read_text_file(char* name, char* data);
};

#endif