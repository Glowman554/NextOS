#include <driver/pit.h>

ProgrammableIntervalTimerDriver::ProgrammableIntervalTimerDriver(uint32_t frequency) : init_port(0x43), counter0_port(0x40) {
	this->frequency = frequency;
}

ProgrammableIntervalTimerDriver::~ProgrammableIntervalTimerDriver() {

}

void ProgrammableIntervalTimerDriver::activate() {
	int counter = 1193182 / this->frequency;
	this->init_port.Write(0x34);

	this->counter0_port.Write(counter & 0xff);
	this->counter0_port.Write(counter >> 8);
}


bool ProgrammableIntervalTimerDriver::is_presend() {
	return true;
}

char* ProgrammableIntervalTimerDriver::get_name() {
	return "pit";
}