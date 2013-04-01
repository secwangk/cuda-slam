#include "parallel.h"

__global__ void test(void *arg)
{}

extern "C"
{
	void run(int argc, char** argv){}
}

/*For testing, not used in slam
__device__ int calc(int arg1, int arg2)
{
	return (arg1+arg2)/2;
}
__global__ void MatAddKernel(int* ma, int* mb, int* mc)
{
	unsigned int tid = blockDim.x * blockIdx.x + threadIdx.x;
//	__syncthreads();
	mc[tid] = calc(ma[tid], mb[tid]);
//	__syncthreads;
}
extern "C" void MatAdd()
{
	unsigned int nBlock=5,nThread=3;
	unsigned int memSize = sizeof(int) * nBlock * nThread;
	int *ma, *mb, *mc;
	
	//host data
	ma = (int*)malloc(memSize);
	mb = (int*)malloc(memSize);
	mc = (int*)malloc(memSize);

	int *dma, *dmb, *dmc;
	
	//device data
	cudaMalloc((void**)&dma, memSize);
	cudaMalloc((void**)&dmb, memSize);
	cudaMalloc((void**)&dmc, memSize);
	for(unsigned int i = 0; i < nBlock * nThread; i++)
		ma[i] =	mb[i] = i;

	cudaMemcpy(dma, ma, memSize, cudaMemcpyHostToDevice);
	cudaMemcpy(dmb, mb, memSize, cudaMemcpyHostToDevice);
	dim3 grid( nBlock, 1, 1);
	dim3 threads( nThread, 1, 1);

	MatAddKernel<<<grid, threads>>>(dma, dmb, dmc);
	cudaMemcpy(mc, dmc, memSize, cudaMemcpyDeviceToHost);
	for(int i=0; i<nBlock;i++)
	  {
		for(int j=0; j<nThread;j++)
	 	{
			int id = i * nThread + j;
			printf("ma:%d  mb:%d  mc%d   ",ma[id],mb[id],mc[id]);
	  	}
		printf("\n");
	 } 
	free(ma);
	free(mb);
	free(mc);
	cudaFree(dma);
	cudaFree(dmb);
	cudaFree(dmc);
}
*/
