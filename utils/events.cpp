#include "utils.hpp"

cudaEvent_t *generate_events(int count){
	cudaEvent_t *events = new cudaEvent_t[count];
	for(int i = 0; i < count; ++i){
		cudaEventCreate(&events[i]);
	}

	return events;
}

void free_events(int count, cudaEvent_t *events){
	for(int i = 0; i < count; ++i){
		cudaEventDestroy(events[i]);
	}
	delete[] events;
}
