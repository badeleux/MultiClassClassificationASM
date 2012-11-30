#ifndef COMMON_H
#define COMMON_H
#include </usr/include/armadillo>

/**
	Structure for passing arguments to multithreaded C++ function.
 */
typedef struct args {
	void *X;
	void *y;
	int from;
	int to;
	void *theta;
} TrainArgs;

/**
	Structure for passing arguments for multithreaded ASM function
 */
typedef struct argsASM {
	float *X;
	int X_row;
	int X_col;
	int *Y;
	int from;
	int to;
	float *allTheta; 
} TrainArgsASM;
#endif
