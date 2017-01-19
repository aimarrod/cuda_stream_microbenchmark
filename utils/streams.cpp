#include "utils.hpp"

cudaStream_t *generate_streams(int count){
	cudaStream_t *streams = new cudaStream_t[count];
	for(int i = 0; i < count; ++i){
		cudaStreamCreate(&streams[i]);
	}

	return streams;
}

void free_streams(int count, cudaStream_t *streams){
	for(int i = 0; i < count; ++i){
		cudaStreamDestroy(streams[i]);
	}
	
	delete[] streams;
}
