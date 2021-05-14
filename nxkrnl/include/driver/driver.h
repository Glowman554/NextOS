#ifndef DRIVER_H
#define DRIVER_H

extern "C" {
	#include <console.h>
	#include <driver/serial.h>
	#include <string.h>
}

#define DRIVER_NUM 265

class Driver{
	public:
		Driver();
		~Driver();
		
		virtual void activate();
		virtual bool is_presend();
		virtual char* get_name();
};

class DriverManager{
	private:
		Driver* drivers[DRIVER_NUM];
		int num_drivers;
	public:
		DriverManager();
		void add_driver(Driver* drv);
		void activate_all(bool force);
		Driver* find_driver_by_name(char* name);
};

#endif
