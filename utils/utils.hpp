#include <cuda_runtime.h>

int **host_alloc(int size, int count);
int **device_alloc(int size, int count);
int **unified_alloc(int size, int count);

void host_free(int count, int **objects);
void device_free(int count, int **objects);
void unified_free(int count, int **objects);

int *generate_sequence(int count, bool shuffle);
bool *generate_flags(int count);

void free_sequence(int *objects);
void free_flags(bool *flags);

cudaEvent_t *generate_events(int count);
void free_events(int count, cudaEvent_t *events);

cudaStream_t *generate_streams(int count);
void free_streams(int count, cudaStream_t *streams);
