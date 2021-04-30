#ifndef NEXTFS_H
#define NEXTFS_H

#include <mem.h>
#include <stdint.h>

#include <string.h>

#define MAGIC 0xf0f0

struct nextfs_header {
	uint16_t magic;
	char label[16];

	uint16_t current_sector;
	uint16_t file_header_index;
} __attribute__((__aligned__((512))));

struct nextfs_file_header {
	char name[16];
	uint16_t start_sector;
	uint16_t length;
} __attribute__((packed));

typedef void (*sector_read)(uint8_t* buffer, uint32_t start_sector, int count, void* data);
typedef void (*sector_write)(uint8_t* buffer, uint32_t start_sector, int count, void* data);


struct nextfs {
	sector_read reader;
	sector_write writer;
	struct nextfs_header header;
	struct nextfs_file_header file_header[100];
};

void init_nextfs(struct nextfs* fs, void* data);
void uninit_nextfs(struct nextfs* fs, void* data);
int is_nextfs(struct nextfs* fs);
void format_nextfs(struct nextfs* fs, char* name);
void print_nextfs(struct nextfs* fs);
void new_file_nextfs(struct nextfs* fs, void* data, char* name, uint8_t* buffer, int size);
void load_file_nextfs(struct nextfs* fs, void* data, char* name, uint8_t* buffer);

#endif