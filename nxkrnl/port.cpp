#include <port.h>
extern "C"{
	#include <io.h>
}

Port8Bit::Port8Bit(uint16_t port){
	portnumber = port;
}

Port8Bit::~Port8Bit(){
	
}

uint8_t Port8Bit::read(){
	return inb(portnumber);
}

void Port8Bit::write(uint8_t data){
	outb(portnumber, data);
}

Port8BitSlow::Port8BitSlow(uint16_t port){
	portnumber = port;
}

Port8BitSlow::~Port8BitSlow(){
	
}

uint8_t Port8BitSlow::read(){
	return inb(portnumber);
}

void Port8BitSlow::write(uint8_t data){
	outb_slow(portnumber, data);
}

Port16Bit::Port16Bit(uint16_t port){
	portnumber = port;
}

Port16Bit::~Port16Bit(){
	
}

uint16_t Port16Bit::read(){
	return inw(portnumber);
}

void Port16Bit::write(uint16_t data){
	outw(portnumber, data);
}

Port32Bit::Port32Bit(uint16_t port){
	portnumber = port;
}

Port32Bit::~Port32Bit(){
	
}

uint32_t Port32Bit::read(){
	return inl(portnumber);
}

void Port32Bit::write(uint32_t data){
	outl(portnumber, data);
}