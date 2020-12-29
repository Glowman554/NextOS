#ifndef CMOS_DEFS_H
#define CMOS_DEFS_H

#define CMOS_FUNCTION_READH 0
#define CMOS_FUNCTION_READM 1
#define CMOS_FUNCTION_READS 2

typedef struct {
	int function;
} cmos_data_t;

#endif