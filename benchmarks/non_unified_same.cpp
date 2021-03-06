#include "benchmarks.hpp" 
#include <iostream>	

#include <chrono>

#include "cuda_profiler_api.h"
#include "../kernels.h"

using namespace std;
using namespace std::chrono;

void run_non_unified_same(int size, int count, int stream_count, int shuffle, int gridSize, int blockSize){
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;

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
	
	t1 = high_resolution_clock::now();

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
	
	t2 = high_resolution_clock::now();

	cudaProfilerStop();

	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout << "Duration: " << duration << "us" << endl;

	free_sequence(order);

	free_events(count, inevents);
	free_events(count, outevents);
	free_events(count, kevents);
	
	free_streams(stream_count, streams);

	host_free(count, host_objects);
	device_free(count, device_objects);
}
