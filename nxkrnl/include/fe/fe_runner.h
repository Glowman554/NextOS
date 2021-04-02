#ifndef FE_RUNNER_H
#define FE_RUNNER_H

#ifdef __cplusplus

// we only want this part in c++

#define EXPOSEC extern "C"

extern "C" {
	#include <fe/fe.h>
	#include <string.h>
}

void run_fe(fe_Code* code_ptr);

#else

#define EXPOSEC

#endif

EXPOSEC void run_fe(char* code);

#endif