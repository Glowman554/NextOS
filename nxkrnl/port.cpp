#include <port.h>
extern "C"{
	#include <io.h>
}

Port8Bit::Port8Bit(uint16_t port){
	portnumber = port;
}

Port8Bit::~Port8Bit(){
	
}

uint8_t Port8Bit::Read(){
	return inb(portnumber);
}

void Port8Bit::Write(uint8_t data){
	outb(portnumber, data);
}

Port8BitSlow::Port8BitSlow(uint16_t port){
	portnumber = port;
}

Port8BitSlow::~Port8BitSlow(){
	
}

uint8_t Port8BitSlow::Read(){
	return inb(portnumber);
}

void Port8BitSlow::Write(uint8_t data){
	outb_slow(portnumber, data);
}

Port16Bit::Port16Bit(uint16_t port){
	portnumber = port;
}

Port16Bit::~Port16Bit(){
	
}

uint16_t Port16Bit::Read(){
	return inw(portnumber);
}

void Port16Bit::Write(uint16_t data){
	outw(portnumber, data);
}

Port32Bit::Port32Bit(uint16_t port){
	portnumber = port;
}

Port32Bit::~Port32Bit(){
	
}

uint32_t Port32Bit::Read(){
	return inl(portnumber);
}

void Port32Bit::Write(uint32_t data){
	outl(portnumber, data);
}