#include <driver/ata.h>

extern "C" bool write_file(char* driver_name, char* file_name, char* file_content) {
	extern DriverManager* global_driver_manager;
	AdvancedTechnologyAttachment* ata = (AdvancedTechnologyAttachment*) global_driver_manager->find_driver_by_name(driver_name);

	if(ata->is_presend()) {
		if(is_nextfs(&ata->fs)) {
			debug_write("Writing file %s to %s!", file_name, driver_name);
			new_file_nextfs(&ata->fs, ata, file_name, (uint8_t*) file_content, strlen(file_content));
			uninit_nextfs(&ata->fs, ata); // also acts like a flush for the file headers
		}
	}
	return false;
}

void ata_reader(uint8_t* buffer, uint32_t start_sector, int count, void* data) {
	AdvancedTechnologyAttachment* ata = (AdvancedTechnologyAttachment*) data;
	debug_write("%s:%d -> 0x%x", ata->get_name(), start_sector, buffer);
	ata->read28(start_sector, buffer, count);
}

void ata_writer(uint8_t* buffer, uint32_t start_sector, int count, void* data) {
	AdvancedTechnologyAttachment* ata = (AdvancedTechnologyAttachment*) data;
	debug_write("%s:%d <- 0x%x", ata->get_name(), start_sector, buffer);
	ata->write28(start_sector, buffer, count);
	ata->flush();
}

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(bool master, uint16_t portBase, char* name): dataPort(portBase), error_port(portBase + 0x1), sector_count_port(portBase + 0x2), lba_low_port(portBase + 0x3), lba_mid_port(portBase + 0x4), lba_hi_port(portBase + 0x5), device_port(portBase + 0x6), command_port(portBase + 0x7), control_port(portBase + 0x206) {
	this->name = name;
	this->master = master;
	this->bytes_per_sector = 512;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment() {
}

bool AdvancedTechnologyAttachment::is_presend() {
	device_port.write(master ? 0xA0 : 0xB0);
	control_port.write(0);
	
	device_port.write(0xA0);
	uint8_t status = command_port.read();
	if(status == 0xFF) {
		return false;
	}
	
	
	device_port.write(master ? 0xA0 : 0xB0);
	sector_count_port.write(0);
	lba_low_port.write(0);
	lba_mid_port.write(0);
	lba_hi_port.write(0);
	command_port.write(0xEC); // identify command
	
	
	status = command_port.read();
	if(status == 0x00) {
		return false;
	}
	
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = command_port.read();
	}
		
	if(status & 0x01) {
		return false;
	}

	
	for(int i = 0; i < 256; i++) {
		uint16_t data = dataPort.read();
		char *text = "  \0";
		text[0] = (data >> 8) & 0xFF;
		text[1] = data & 0xFF;
	}

	return true;
}

char* AdvancedTechnologyAttachment::get_name() {
	return this->name;
}

void AdvancedTechnologyAttachment::activate() {

	this->fs.reader = ata_reader;
	this->fs.writer = ata_writer;

	init_nextfs(&this->fs, this);

	debug_write("Is nextfs: %s!", is_nextfs(&this->fs) ? "true" : "false");

	print_nextfs(&this->fs);
}

void AdvancedTechnologyAttachment::read28(uint32_t sector, uint8_t* data, int count) {
	if(sector & 0xF0000000) {
		return;
	}
	if(count > bytes_per_sector) {
		return;
	}
	
	device_port.write((master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	error_port.write(0);
	sector_count_port.write(1);
	
	lba_low_port.write(sector & 0x000000FF);
	lba_mid_port.write((sector & 0x0000FF00) >> 8);
	lba_hi_port.write((sector & 0x00FF0000) >> 16);
	command_port.write(0x20);
	
	
	
	uint8_t status = command_port.read();
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = command_port.read();
	}
	
	if(status & 0x01) {
		return;
	}
	
	for(uint16_t i = 0; i < count; i += 2) {
		uint16_t wdata = dataPort.read();
		
		data[i] = wdata & 0x00FF;
		if(i + 1 < count) {
			data[i + 1] = (wdata >> 8) & 0x00FF;
		}
	}
	
	for(uint16_t i = count + (count % 2); i < bytes_per_sector; i+= 2) {
		dataPort.read();
	}
}

void AdvancedTechnologyAttachment::write28(uint32_t sectorNum, uint8_t* data, uint32_t count) {
	if(sectorNum > 0x0FFFFFFF) {
		return;
	}
	if(count > bytes_per_sector) {
		return;
	}
	
	
	device_port.write((master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24));
	error_port.write(0);
	sector_count_port.write(1);
	lba_low_port.write(sectorNum & 0x000000FF);
	lba_mid_port.write((sectorNum & 0x0000FF00) >> 8);
	lba_hi_port.write((sectorNum & 0x00FF0000) >> 16);
	command_port.write(0x30);


	for(int i = 0; i < (int) count; i += 2) {
		uint16_t wdata = data[i];
		if(i + 1 < (int) count) {
			wdata |= ((uint16_t)data[i + 1]) << 8;
		}
		dataPort.write(wdata);
	}
	
	for(int i = count + (count % 2); i < 512; i += 2) {
		dataPort.write(0x0000);
	}

}

void AdvancedTechnologyAttachment::flush() {
	device_port.write(master ? 0xE0 : 0xF0);
	command_port.write(0xE7);

	uint8_t status = command_port.read();
	if(status == 0x00) {
		return;
	}
	
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = command_port.read();
	}
		
	if(status & 0x01) {
		return;
	}
}
   