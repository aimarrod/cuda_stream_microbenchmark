#include "benchmarks.hpp" 
#include <iostream>	

#include <time.h>

#include "cuda_profiler_api.h"
#include "../kernels.h"

void run_non_unified_same(int size, int count, int stream_count, int shuffle, int gridSize, int blockSize){
	int **host_objects = host_alloc(size, count);
	int **device_objects = device_alloc(size, count);
	
	int *order = generate_sequence(count, shuffle);

	cudaEvent_t *inevents = generate_events(count);
	cudaEvent_t *kevents = generate_events(count);
	cudaEvent_t *outevents = generate_events(count);

	cudaStream_t *streams = generate_streams(stream_count);
	int curr_stream = 0;

	clock_t start_t, end_t, total_t;

	cudaProfilerStart();
	
	start_t = clock();

	for(int i = 0; i < count; ++i){
		cudaMemcpyAsync(
			device_objects[ order[i] ],
			host_objects[ order[i] ],
			size * sizeof(int),
			cudaMemcpyHostToDevice,
			streams[ curr_stream ]
		);

		cudaEventRecord(inevents[ order[i] ], streams[curr_stream]);

		assign_order( device_objects[ order[i] ], gridSize, blockSize, streams[curr_stream] );

		cudaEventRecord(kevents[ order[i] ], streams[curr_stream]);		

		cudaMemcpyAsync(
			host_objects[ order[i] ],
			device_objects[ order[i] ],
			size * sizeof(int),
			cudaMemcpyDeviceToHost,
			streams[ curr_stream ]
		);

		cudaEventRecord(outevents[ order[i] ], streams[curr_stream]);
	
		curr_stream = (curr_stream + 1) % stream_count;	
	}	

	cudaDeviceSynchronize();
	
	end_t = clock();

	cudaProfilerStop();

	total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
	std::cout << "Total time: " << total_t << std::endl;

	free_sequence(order);

	free_events(count, inevents);
	free_events(count, outevents);
	free_events(count, kevents);
	
	free_streams(count, streams);

	host_free(count, host_objects);
	device_free(count, device_objects);
}
