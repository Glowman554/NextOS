#ifndef FE_RUNNER_H
#define FE_RUNNER_H

extern "C" {
	#include <fe/fe.h>
	#include <string.h>
}

void run_fe(char* code);
void run_fe(fe_Code* code_ptr);

#endif