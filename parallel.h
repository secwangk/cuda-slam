//copywrite: Wang Kai
//Here are some parallized functions for low.c and high.c
//the most time consuming funcitons are:
// HighLocalize, Localize,
// HighUpdateAncstry, UpdateAncestry
#ifndef _PARALLEL_H_
#define _PARALLEL_H_
#include "low.h"
#include "high.h"
#include <cuda_runtime.h>
#include <math_functions.h>
#include <helper_cuda.h>
#include <helper_functions.h>

__global__ void test(void *arg);

extern "C"
{
	void run(int argc, char** argv);
	
	/*For testing, not used in slam
	void MatAdd();*/
}

#endif
