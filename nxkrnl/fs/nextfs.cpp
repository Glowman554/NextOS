#include <fs/nextfs.h>

NextFS::NextFS(AdvancedTechnologyAttachment* ata) {
	this->ata = ata;

	this->header = (nextfs_header*) pmm_alloc();
	memset(this->header, 0, 4096);
	this->file_array = (nextfs_file_header*) pmm_alloc();
	memset(this->file_array, 0, 4096);

	this->ata->read28(17, (uint8_t*) this->header, 512);
	for(int i = 0; i < 4; i++) {
		this->ata->read28(18 + i, (uint8_t*) this->file_array + 512 * i, 512);
	}
}

NextFS::~NextFS() {
	this->ata->write28(17, (uint8_t*) this->header, 512);
	this->ata->flush();
	for(int i = 0; i < 4; i++) {
		this->ata->write28(18 + i, (uint8_t*) this->file_array + 512 * i, 512);
		this->ata->flush();
	}
	pmm_free(this->header);
	pmm_free(this->file_array);
}

void NextFS::print_fs_info() {
	debug_write("[%s] current sector: %d, file header idx: %d!", this->header->name, this->header->current_sector, this->header->file_header_index);

	for(int i = 1; i < this->header->file_header_index; i++) {
		debug_write("[%s] length: %d, start sector: %d!", this->file_array[i].name, this->file_array[i].length, this->file_array[i].start_sector);
	}
}

bool NextFS::is_next_fs() {
	return this->header->magic == MAGIC;
}

void NextFS::format(char* name) {
	this->header->magic = MAGIC;
	this->header->current_sector = 22;
	this->header->file_header_index = 1;
	strcpy(this->header->name, name);
}

void NextFS::new_text_file(char* name, char* data) {
	strcpy(this->file_array[this->header->file_header_index].name, name);
	this->file_array[this->header->file_header_index].start_sector = this->header->current_sector;
	this->file_array[this->header->file_header_index].length = strlen(data) / 512;

	int sectors_needed = this->file_array[this->header->file_header_index].length / 512;

	for(int i = 0; i < sectors_needed + 1; i++) {
		this->ata->write28(this->header->current_sector, (uint8_t*) data + 512 * i, 512);
		this->header->current_sector++;
		this->ata->flush();
	}

	this->header->file_header_index++;
} 

void NextFS::read_text_file(char* name, char* data) {
	for(int i = 1; i < this->header->file_header_index; i++) {
		if(strcmp(this->file_array[i].name, name) == 0) {
			debug_write_lame("Found file in fs!");

			int sectors_needed = this->file_array[i].length;

			for(int j = 0; j < sectors_needed + 1; j++) {
				this->ata->read28(this->file_array[i].start_sector + j, (uint8_t*) data + 512 * j, 512);
			}
		} 
	}
}