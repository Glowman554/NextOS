#ifndef PIT_H
#define PIT_H

#include <stdint.h>
#include <driver/driver.h>
#include <port.h>
#include <interrupt_handler.h>

extern "C" {
	#include <console.h>
}


class ProgrammableIntervalTimerDriver : public Driver {
	private:
		Port8Bit init_port;
		Port8Bit counter0_port;
		
		uint32_t frequency;
		
	public:
		ProgrammableIntervalTimerDriver(uint32_t frequency);
		~ProgrammableIntervalTimerDriver();
		virtual void activate();
		virtual bool is_presend();
		virtual char* get_name();
};

#endif