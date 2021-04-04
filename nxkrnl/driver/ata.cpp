#include <driver/ata.h>

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(bool master, uint16_t portBase, char* name): dataPort(portBase), errorPort(portBase + 0x1), sectorCountPort(portBase + 0x2), lbaLowPort(portBase + 0x3), lbaMidPort(portBase + 0x4), lbaHiPort(portBase + 0x5), devicePort(portBase + 0x6), commandPort(portBase + 0x7), controlPort(portBase + 0x206) {
	this->name = name;
	this->master = master;
	this->bytesPerSector = 512;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment() {
}

bool AdvancedTechnologyAttachment::is_presend() {
	devicePort.Write(master ? 0xA0 : 0xB0);
	controlPort.Write(0);
	
	devicePort.Write(0xA0);
	uint8_t status = commandPort.Read();
	if(status == 0xFF) {
		return false;
	}
	
	
	devicePort.Write(master ? 0xA0 : 0xB0);
	sectorCountPort.Write(0);
	lbaLowPort.Write(0);
	lbaMidPort.Write(0);
	lbaHiPort.Write(0);
	commandPort.Write(0xEC); // identify command
	
	
	status = commandPort.Read();
	if(status == 0x00) {
		return false;
	}
	
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = commandPort.Read();
	}
		
	if(status & 0x01) {
		return false;
	}

	
	for(int i = 0; i < 256; i++) {
		uint16_t data = dataPort.Read();
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
}

void AdvancedTechnologyAttachment::Read28(uint32_t sector, uint8_t* data, int count) {
	if(sector & 0xF0000000) {
		return;
	}
	if(count > bytesPerSector) {
		return;
	}
	
	devicePort.Write((master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	errorPort.Write(0);
	sectorCountPort.Write(1);
	
	lbaLowPort.Write(sector & 0x000000FF);
	lbaMidPort.Write((sector & 0x0000FF00) >> 8);
	lbaHiPort.Write((sector & 0x00FF0000) >> 16);
	commandPort.Write(0x20);
	
	
	
	uint8_t status = commandPort.Read();
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = commandPort.Read();
	}
	
	if(status & 0x01) {
		return;
	}
	
	for(uint16_t i = 0; i < count; i += 2) {
		uint16_t wdata = dataPort.Read();
		
		data[i] = wdata & 0x00FF;
		if(i + 1 < count) {
			data[i + 1] = (wdata >> 8) & 0x00FF;
		}
	}
	
	for(uint16_t i = count + (count % 2); i < bytesPerSector; i+= 2) {
		dataPort.Read();
	}
}

void AdvancedTechnologyAttachment::Write28(uint32_t sectorNum, uint8_t* data, uint32_t count) {
	if(sectorNum > 0x0FFFFFFF) {
		return;
	}
	if(count > bytesPerSector) {
		return;
	}
	
	
	devicePort.Write((master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24));
	errorPort.Write(0);
	sectorCountPort.Write(1);
	lbaLowPort.Write(sectorNum & 0x000000FF);
	lbaMidPort.Write((sectorNum & 0x0000FF00) >> 8);
	lbaLowPort.Write((sectorNum & 0x00FF0000) >> 16);
	commandPort.Write(0x30);


	for(int i = 0; i < (int) count; i += 2) {
		uint16_t wdata = data[i];
		if(i + 1 < (int) count) {
			wdata |= ((uint16_t)data[i + 1]) << 8;
		}
		dataPort.Write(wdata);
	}
	
	for(int i = count + (count % 2); i < 512; i += 2) {
		dataPort.Write(0x0000);
	}

}

void AdvancedTechnologyAttachment::Flush() {
	devicePort.Write(master ? 0xE0 : 0xF0);
	commandPort.Write(0xE7);

	uint8_t status = commandPort.Read();
	if(status == 0x00) {
		return;
	}
	
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = commandPort.Read();
	}
		
	if(status & 0x01) {
		return;
	}
}
   