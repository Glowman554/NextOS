#include <driver/driver.h>

Driver::Driver(){
	
}

Driver::~Driver(){
	
}

void Driver::activate(){
	
}

bool Driver::is_presend() {
	return true;
}

char* Driver::get_name() {
	return "uwu";
}

DriverManager::DriverManager(){
	num_drivers = 0;
}

void DriverManager::AddDriver(Driver* drv){
	drivers[num_drivers] = drv;
	num_drivers++;
}

void DriverManager::ActivateAll(bool force){
	for(int i = 0; i < num_drivers; i++) {
		kprintf("Loading %s driver at index %d!", drivers[i]->get_name(), i);
		if(force) {
			drivers[i]->activate();
			setx(80 - 8);
			kprintf("[");
			setcolor(BACKGROUND_BLACK | FOREGROUND_CYAN);
			kprintf("force");
			setcolor(BACKGROUND_BLACK | FOREGROUND_WHITE);
			kprintf("]\n");
		} else {
			if(drivers[i]->is_presend()) {
				drivers[i]->activate();
				setx(80 - 5);
				kprintf("[");
				setcolor(BACKGROUND_BLACK | FOREGROUND_GREEN);
				kprintf("ok");
				setcolor(BACKGROUND_BLACK | FOREGROUND_WHITE);
				kprintf("]\n");
			} else {
				setx(80 - 7);
				kprintf("[");
				setcolor(BACKGROUND_BLACK | FOREGROUND_RED);
				kprintf("fail");
				setcolor(BACKGROUND_BLACK | FOREGROUND_WHITE);
				kprintf("]\n");
			}
		}
	}
}