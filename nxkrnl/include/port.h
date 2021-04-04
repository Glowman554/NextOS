#ifndef PORT_H
#define PORT_H
#include <stdint.h>

class Port8Bit{
	private:
		uint16_t portnumber;
	public:
		Port8Bit(uint16_t port);
		~Port8Bit();
		
		void write(uint8_t data);
		uint8_t read();
};

class Port8BitSlow{
	private:
		uint16_t portnumber;
	public:
		Port8BitSlow(uint16_t port);
		~Port8BitSlow();
		
		void write(uint8_t data);
		uint8_t read();
};

class Port16Bit{
	private:
		uint16_t portnumber;
	public:
		Port16Bit(uint16_t port);
		~Port16Bit();
		
		void write(uint16_t data);
		uint16_t read();
};

class Port32Bit{
	private:
		uint16_t portnumber;
	public:
		Port32Bit(uint16_t port);
		~Port32Bit();
		
		void write(uint32_t data);
		uint32_t read();
};

#endif