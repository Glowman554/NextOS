#include <driver/ata.h>

extern "C" bool write_file(char* driver_name, char* file_name, char* file_content) {
	extern DriverManager* global_driver_manager;
	AdvancedTechnologyAttachment* ata = (AdvancedTechnologyAttachment*) global_driver_manager->find_driver_by_name(driver_name);

	if(ata->is_presend()) {
		NextFS fs = NextFS(ata);
		if(fs.is_next_fs()) {
			char buffer[1000];
			sprintf(buffer, "Writing file %s to %s!", file_name, driver_name);
			debug_write(buffer);

			fs.new_text_file(file_name, file_content);
			return true;
		}
	}
	return false;
}


AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(bool master, uint16_t portBase, char* name): dataPort(portBase), errorPort(portBase + 0x1), sectorCountPort(portBase + 0x2), lbaLowPort(portBase + 0x3), lbaMidPort(portBase + 0x4), lbaHiPort(portBase + 0x5), devicePort(portBase + 0x6), commandPort(portBase + 0x7), controlPort(portBase + 0x206) {
	this->name = name;
	this->master = master;
	this->bytesPerSector = 512;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment() {
}

bool AdvancedTechnologyAttachment::is_presend() {
	devicePort.write(master ? 0xA0 : 0xB0);
	controlPort.write(0);
	
	devicePort.write(0xA0);
	uint8_t status = commandPort.read();
	if(status == 0xFF) {
		return false;
	}
	
	
	devicePort.write(master ? 0xA0 : 0xB0);
	sectorCountPort.write(0);
	lbaLowPort.write(0);
	lbaMidPort.write(0);
	lbaHiPort.write(0);
	commandPort.write(0xEC); // identify command
	
	
	status = commandPort.read();
	if(status == 0x00) {
		return false;
	}
	
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = commandPort.read();
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

	NextFS fs = NextFS(this);

	char buffer[1000];
	sprintf(buffer, "Is nextfs: %s!", fs.is_next_fs() ? "true" : "false");
	debug_write(buffer);

	if(!fs.is_next_fs()) {
		if(global_kernel_info.auto_format) {
			fs.format(global_kernel_info.format_label);
		}
	}
	
	fs.print_fs_info();
}

void AdvancedTechnologyAttachment::Read28(uint32_t sector, uint8_t* data, int count) {
	if(sector & 0xF0000000) {
		return;
	}
	if(count > bytesPerSector) {
		return;
	}
	
	devicePort.write((master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	errorPort.write(0);
	sectorCountPort.write(1);
	
	lbaLowPort.write(sector & 0x000000FF);
	lbaMidPort.write((sector & 0x0000FF00) >> 8);
	lbaHiPort.write((sector & 0x00FF0000) >> 16);
	commandPort.write(0x20);
	
	
	
	uint8_t status = commandPort.read();
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = commandPort.read();
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
	
	for(uint16_t i = count + (count % 2); i < bytesPerSector; i+= 2) {
		dataPort.read();
	}
}

void AdvancedTechnologyAttachment::Write28(uint32_t sectorNum, uint8_t* data, uint32_t count) {
	if(sectorNum > 0x0FFFFFFF) {
		return;
	}
	if(count > bytesPerSector) {
		return;
	}
	
	
	devicePort.write((master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24));
	errorPort.write(0);
	sectorCountPort.write(1);
	lbaLowPort.write(sectorNum & 0x000000FF);
	lbaMidPort.write((sectorNum & 0x0000FF00) >> 8);
	lbaHiPort.write((sectorNum & 0x00FF0000) >> 16);
	commandPort.write(0x30);


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

void AdvancedTechnologyAttachment::Flush() {
	devicePort.write(master ? 0xE0 : 0xF0);
	commandPort.write(0xE7);

	uint8_t status = commandPort.read();
	if(status == 0x00) {
		return;
	}
	
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = commandPort.read();
	}
		
	if(status & 0x01) {
		return;
	}
}
   