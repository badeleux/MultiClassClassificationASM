#ifndef COMMON_H
#define COMMON_H
#include </usr/include/armadillo>

typedef struct args {
	void *X;
	void *y;
	int from;
	int to;
	void *theta;
} TrainArgs;

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
