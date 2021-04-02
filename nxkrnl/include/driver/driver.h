#ifndef DRIVER_H
#define DRIVER_H

extern "C" {
	#include <console.h>
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
		void AddDriver(Driver* drv);
		void ActivateAll(bool force);
};

#endif