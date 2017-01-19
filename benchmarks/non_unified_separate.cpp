#include "benchmarks.hpp" 
#include <iostream>	

#include <chrono>

#include "cuda_profiler_api.h"
#include "../kernels.h"

using namespace std;
using namespace std::chrono;

void run_non_unified_separate(int size, int count, int stream_count, int shuffle, int gridSize, int blockSize){
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;

	int **host_objects = host_alloc(size, count);
	int **device_objects = device_alloc(size, count);
	
	int *inorder = generate_sequence(count, shuffle);
	int *korder = generate_sequence(count, shuffle);
	int *outorder = generate_sequence(count, shuffle);

	cudaEvent_t *inevents = generate_events(count);
	cudaEvent_t *kevents = generate_events(count);
	cudaEvent_t *outevents = generate_events(count);

	bool *insubmitted = generate_flags(count);
	bool *ksubmitted = generate_flags(count);
	bool *outsubmitted = generate_flags(count);

	cudaStream_t *kstreams = generate_streams(stream_count);
	int curr_kstream = 0;
	
	cudaStream_t *tstreams = generate_streams(2);
	int in_tstream = 0;
	int out_tstream = 1;

	bool work_remains = false;
	int i = 0;

	clock_t start_t, end_t, total_t;

	t1 = high_resolution_clock::now();

	cudaProfilerStart();	

	while(1){

		if(!insubmitted[ inorder[i] ]){
			cudaMemcpyAsync(
				device_objects[ inorder[i] ],
				host_objects[ inorder[i] ],
				size * sizeof(int),
				cudaMemcpyHostToDevice,
				tstreams[ in_tstream ]
			);

			cudaEventRecord(inevents[ inorder[i] ], tstreams[ in_tstream ]);
			insubmitted[ inorder[i] ] = true;
		}

		if(!ksubmitted[ korder[i] ]){
			if(insubmitted[ korder[i] ] && cudaEventQuery(inevents[ korder[i] ]) == cudaSuccess){
				assign_order( device_objects[ korder[i] ], gridSize, blockSize, kstreams[curr_kstream] );
			
				cudaEventRecord(kevents[ korder[i] ], kstreams[curr_kstream]);
				curr_kstream = (curr_kstream + 1) % stream_count;
				ksubmitted[ korder[i] ] = true;
			} else {
				work_remains = true;	
			}
		}

		if(!outsubmitted[ outorder[i]]){
			if(ksubmitted[ outorder[i] ] && cudaEventQuery(kevents[ outorder[i] ]) == cudaSuccess ){			
				cudaMemcpyAsync(
					host_objects[ outorder[i] ],
					device_objects[ outorder[i] ],
					size * sizeof(int),
					cudaMemcpyDeviceToHost,
					tstreams[ out_tstream ]
				);
							
				cudaEventRecord(outevents[ outorder[i] ], tstreams[out_tstream]);
				outsubmitted[ outorder[i] ] = true;
			} else {
				work_remains = true;
			}
		}

		if(i == count-1){ //Last iteration check
			if(work_remains){
				i = 0;
				work_remains = false;
			} else {
				break;
			}
		} else {
			i = (i+1)%count;
		}
	}


	cudaDeviceSynchronize();

	t2 = high_resolution_clock::now();
	
	cudaProfilerStop();

	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout << "Duration: " << duration << "us" << endl;

	free_sequence(inorder);
	free_sequence(korder);
	free_sequence(outorder);

	free_flags(insubmitted);
	free_flags(ksubmitted);
	free_flags(outsubmitted);

	free_events(count, inevents);
	free_events(count, outevents);
	free_events(count, kevents);
	
	free_streams(stream_count, kstreams);
	free_streams(2, tstreams);

	host_free(count, host_objects);
	device_free(count, device_objects);
}
