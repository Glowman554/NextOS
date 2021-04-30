#include <fs/nextfs.h>


void init_nextfs(struct nextfs* fs, void* data) {
	fs->reader((uint8_t*) &fs->header, 17, 512, data);
	for (int i = 0; i < 4; i++) {
		fs->reader((uint8_t*) fs->file_header + 512 * i, i + 18, 512, data);
	}
}

void uninit_nextfs(struct nextfs* fs, void* data) {
	fs->writer((uint8_t*) &fs->header, 17, 512, data);
	for (int i = 0; i < 4; i++) {
		fs->writer((uint8_t*) fs->file_header + 512 * i, i + 18, 512, data);
	}
}

int is_nextfs(struct nextfs* fs) {
	return fs->header.magic == MAGIC;
}

void format_nextfs(struct nextfs* fs, char* name) {
	fs->header.magic = MAGIC;
	fs->header.current_sector = 22;
	fs->header.file_header_index = 1;
	strcpy(fs->header.label, name);
}

void print_nextfs(struct nextfs* fs) {
	debug_write("[%s] current sector: %d, file header idx: %d!", fs->header.label, fs->header.current_sector, fs->header.file_header_index);

	for (int i = 1; i < fs->header.file_header_index; i++) {
		debug_write("[%s] length: %d, start sector: %d!", fs->file_header[i].name, fs->file_header[i].length, fs->file_header[i].start_sector);
	}
	
}

void new_file_nextfs(struct nextfs* fs, void* data, char* name, uint8_t* buffer, int size) {
	strcpy(fs->file_header[fs->header.file_header_index].name, name);
	fs->file_header[fs->header.file_header_index].start_sector = fs->header.current_sector;
	fs->file_header[fs->header.file_header_index].length = size / 512;

	int sectors_needed = size / 512;
	for (int i = 0; i < sectors_needed + 1; i++) {
		fs->writer(buffer + 512 * i, fs->header.current_sector, 512, data);
		fs->header.current_sector++;
	}
	fs->header.file_header_index++;
}

void load_file_nextfs(struct nextfs* fs, void* data, char* name, uint8_t* buffer) {
	for (int i = 0; i < fs->header.file_header_index; i++) {
		if(strcmp(fs->file_header[i].name, name) == 0) {

			int sectors_needed = fs->file_header[i].length;

			for (int j = 0; j < sectors_needed + 1; j++) {
				fs->reader((uint8_t*) buffer + 512 * i, fs->file_header[i].start_sector + j, 512, data);
			}
		}
	}
}
