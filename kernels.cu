#include "kernels.h"

__global__ void assign_order_kernel(int *object){
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	object[id] = id;
}

void assign_order(int *objects, int gridSize, int blockSize, cudaStream_t stream){
	assign_order_kernel<<<gridSize, blockSize, 0, stream>>>(objects);
}


