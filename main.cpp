#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <cuda_runtime.h>

#include <benchmarks.hpp>

int main(int argc, char* argv[]){

	//Default values
	int size = 100;
	int count = 100;
	int stream_count = 10;
	int shuffle_exec = 0;
	int device = 0;

	int mode = 0;
	
	static struct option long_options[] =
	{
		/* Optional Arguments */
		{"streamnum", required_argument, 0, 'u'},
		{"shuffle", no_argument, &shuffle_exec, 1},
		/* Mode flags */
		{"non-unified-separate", no_argument, &mode, 0},
		{"non-unified-same", no_argument, &mode, 1},
		{"unified-separate", no_argument, &mode, 2},
		{"unified-same", no_argument, &mode, 3},
		/* Termination */
		{0, 0, 0, 0}
	};

	int option_index = 0;
	int option = 0;
	while((option = getopt_long(argc, argv, "s:n:d:", long_options, &option_index)) != -1){
		switch(option){
			case 0:
				break;
			case 'u':
				stream_count = atoi(optarg);
				break;
			case 's':
				size = atoi(optarg);
				break;
			case 'n':
				count = atoi(optarg);
				break;
			case 'd':
				device = atoi(optarg);
				break;
			default:
				abort();
				break;
		}	
	}

	//Pick a GPU
	cudaDeviceProp properties;
	cudaGetDeviceProperties(&properties, device);
	int blockSize = size;
	int gridSize = 1;
	if(properties.maxThreadsPerBlock < size){
		blockSize = properties.maxThreadsPerBlock;
		gridSize = 1 + ((size -1)/ properties.maxThreadsPerBlock); 
	}

	std::cout << "Running Stream Benchmark" << std::endl;
	std::cout << "\tObject size: " << size << std::endl;
	std::cout << "\tNumber of Objects: " << count << std::endl;
	std::cout << "\tNumber of Streams: " << stream_count << std::endl;
	
	std::cout << "\tShuffle: ";
	if(shuffle_exec) std::cout << "yes";
	else std::cout << "no";
	std::cout << std::endl;	

	std::cout << "\tGPU: " << properties.name << std::endl;
	std::cout << "\tBlock Size: " << blockSize << std::endl;
	std::cout << "\tGrid Size: " << gridSize << std::endl;

	std::cout << "\tMode: ";
	if(mode == 0) std::cout << "\n\t\tSeparate memory \n\t\tsynchronization using events";
	else if(mode == 1) std::cout << "\n\t\tSeparate memory \n\t\tsynchonization using streams";
	else if(mode == 2) std::cout << "\n\t\tCUDA Unified memory \n\t\tsynchronization using events";
	else if(mode == 3) std::cout << "\n\t\tCUDA Unified memory \n\t\tsynchronization using streams";
	std::cout << std::endl;
	

	/*
	 * RUN BENCHMARK
	 */ 
	if(mode == 0) run_non_unified_separate(size, count, stream_count, shuffle_exec, gridSize, blockSize);
	else if(mode == 1) run_non_unified_same(size, count, stream_count, shuffle_exec, gridSize, blockSize);
	else if(mode == 2) run_unified_separate(size, count, stream_count, shuffle_exec, gridSize, blockSize);
	else if(mode == 3) run_unified_same(size, count, stream_count, shuffle_exec, gridSize, blockSize);
}
