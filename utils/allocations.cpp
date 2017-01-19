#include "utils.hpp"

int **host_alloc(int size, int count){
	int **host_objects = new int *[count];
	for(int i = 0; i < count; ++i){
		host_objects[i] = new int[size];
	}

	return host_objects;
}

int **device_alloc(int size, int count){
	int **device_objects = new int *[count];
	for(int i = 0; i < count; ++i){
		cudaMalloc(&device_objects[i], size * sizeof(int));
	}

	return device_objects;
}

int **unified_alloc(int size, int count){
	int **objects = new int*[count];
	for(int i = 0; i < count; ++i){
		cudaMallocManaged( &objects[i], size * sizeof(int));
	}

	return objects;
}

void host_free(int count, int **host_objects){
	for(int i = 0; i < count; ++i){
		delete[] host_objects[i];
	}
	delete[] host_objects;
}

void device_free(int count, int **device_objects){
	for(int i = 0; i < count; ++i){
		cudaFree(device_objects[i]);
	}
	delete[] device_objects;
}

void unified_free(int count, int **objects){
	for(int i = 0; i < count; ++i){
		cudaFree(objects[i]);
	}
	delete[] objects;
}
