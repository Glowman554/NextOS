#ifndef PCI_H
#define PCI_H

extern "C"{
	#include <console.h>
	#include <driver/serial.h>
}

#include <stdint.h>
#include <port.h>

enum BaseAddressRegisterType{
	MemoryMapping = 0,
	InputOutput = 1
};

class BaseAddressRegister{
	public:
		bool prefetchable;
		uint8_t* address;
		BaseAddressRegisterType type;
};

class PeripheralComponentInterconnectDeviceDescriptor{
	public:
		uint32_t port_base;
		uint32_t interrupt;
		uint16_t bus;
		uint16_t device;
		uint16_t function;
		uint16_t vendor_id;
		uint16_t device_id;
		uint8_t class_id;
		uint8_t subclass_id;
		uint8_t interface_id;
		uint8_t revision;
};

class PeripheralComponentInterconnectController{
	private:
		Port32Bit data_port;
		Port32Bit command_port;
	public:
		PeripheralComponentInterconnectController();
		~PeripheralComponentInterconnectController();
		
		uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
		void write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
		bool device_has_functions(uint16_t bus, uint16_t device);
		PeripheralComponentInterconnectDeviceDescriptor get_device_descriptor(uint16_t bus, uint16_t device, uint16_t function);
		BaseAddressRegister get_base_addres_register(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);
		void print_devices();
	
};

#endif