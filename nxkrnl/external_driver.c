#include <external_driver.h>


driver_info_t driver_infos[256];
int driver_index = 0;

int init_driver(char* name, driver_handler_ptr driver_handler, bool no_free) {
	//kprintf("Registering driver handler %d at 0x%x\n", driver_index, driver_handler);

	driver_info_t driver;
	driver.driver_handler = driver_handler;
	driver.name = name;
	driver_infos[driver_index] = driver;
	driver_index++;

	if(no_free) {
		task_states[current_task].no_free = true;
	}

	debug_write("Registering driver handler at 0x%x with id %d and name %s!", (unsigned int) driver_handler, driver_index - 1, name);

	return driver_index - 1;
}

int call_driver_handler(int id, void* data) {
	return (*(driver_infos[id].driver_handler))(data);
}

int find_driver_by_name(char* name) {
	for(int i = 0; i < 256; i++) {

		if(strcmp(name, driver_infos[i].name)==0) {
			return i;
		}
	}
	return -1;
}