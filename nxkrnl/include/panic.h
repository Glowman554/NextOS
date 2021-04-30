#ifndef PANIC_H
#define PANIC_H

#include <console.h>
#include <interrupts.h>
#include <driver/vga.h>
#include <driver/serial.h>
#include <stdbool.h>
#include <system.h>
#include <task.h>

char* get_exception_name(int exc);
void kernel_yeet_cpu(struct cpu_state* cpu);
void kernel_yeet_str(char* why);

void __kdb();

#define yeet0 "  _  ________ _____  _   _ ______ _       __     ________ ______ _______ \n"
#define yeet1 " | |/ /  ____|  __ \\| \\ | |  ____| |      \\ \\   / /  ____|  ____|__   __|\n"
#define yeet2 " | ' /| |__  | |__) |  \\| | |__  | |       \\ \\_/ /| |__  | |__     | |   \n"
#define yeet3 " |  < |  __| |  _  /| . ` |  __| | |        \\   / |  __| |  __|    | |   \n"
#define yeet4 " | . \\| |____| | \\ \\| |\\  | |____| |____     | |  | |____| |____   | |   \n"
#define yeet5 " |_|\\_\\______|_|  \\_\\_| \\_|______|______|    |_|  |______|______|  |_|   \n"


#endif