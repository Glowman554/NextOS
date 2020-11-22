#ifndef DRIVER_H
#define DRIVER_H

#define DRIVER_NUM 265

class Driver{
	public:
		Driver();
		~Driver();
		
		virtual void Activate();
};

class DriverManager{
	private:
		Driver* drivers[DRIVER_NUM];
		int num_drivers;
	public:
		DriverManager();
		void AddDriver(Driver*);
		void ActivateAll();
};

#endif