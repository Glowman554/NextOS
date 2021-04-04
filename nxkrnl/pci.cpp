#include <pci.h>

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController() : dataPort(0xcfc), commandPort(0xcf8){
	
}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController(){
	
}

uint32_t PeripheralComponentInterconnectController::read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset){
	uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registeroffset & 0xFC);
	commandPort.write(id);
	uint32_t result = dataPort.read();
	return result >> (8 * (registeroffset % 4));
}
void PeripheralComponentInterconnectController::write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value){
	uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registeroffset & 0xFC);
	commandPort.write(id);
	dataPort.write(value);
}
bool PeripheralComponentInterconnectController::device_has_functions(uint16_t bus, uint16_t device){
	return read(bus, device, 0, 0x0e) & (1<<7);
}
PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::get_device_descriptor(uint16_t bus, uint16_t device, uint16_t function){
	PeripheralComponentInterconnectDeviceDescriptor result;
	result.bus = bus;
	result.device = device;
	result.function = function;
	result.vendor_id = read(bus, device, function, 0x00);
	result.device_id = read(bus, device, function, 0x02);
	result.class_id = read(bus, device, function, 0x0b);
	result.subclass_id = read(bus, device, function, 0x0a);
	result.interface_id = read(bus, device, function, 0x09);
	result.revision = read(bus, device, function, 0x08);
	result.interrupt = read(bus, device, function, 0x3c);
	return result;
}
BaseAddressRegister PeripheralComponentInterconnectController::get_base_addres_register(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
	BaseAddressRegister result;
	uint32_t headertype = read(bus, device, function, 0x0E) & 0x7F;
	int maxBARs = 6 - (4*headertype);
	if(bar >= maxBARs)
		return result;

	uint32_t bar_value = read(bus, device, function, 0x10 + 4*bar);
	result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;

	if(result.type == MemoryMapping){
		switch((bar_value >> 1) & 0x3){
			case 0: 
			case 1: 
			case 2: 
				break;
		}
	} else {
		result.address = (uint8_t*)(bar_value & ~0x3);
		result.prefetchable = false;
	}
	return result;
}
void PeripheralComponentInterconnectController::print_devices(){
	debug_write("Found PCI Devices:");
	for(int bus = 0; bus < 8; bus++){
		for(int device = 0; device < 32; device++){
			int numFunctions = device_has_functions(bus, device) ? 8 : 1;
			for(int function = 0; function < numFunctions; function++){
				PeripheralComponentInterconnectDeviceDescriptor dev = get_device_descriptor(bus, device, function);
				if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
					continue;
				
				kprintf_serial("bus: 0x%x, device: 0x%x, function: 0x%x, vendor: 0x%x, deviceid 0x%x\n", dev.bus, dev.device, dev.function, dev.vendor_id, dev.device_id);
			}
		}
	}
	return;
}