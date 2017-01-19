#include <utils.hpp>

void run_non_unified_separate(int size, int count, int stream_count, int shuffle, int gridSize, int blockSize);
void run_non_unified_same(int size, int count, int stream_count, int shuffle, int gridSize, int blockSize);
void run_unified_separate(int size, int count, int stream_count, int shuffle, int gridSize, int blockSize);
void run_unified_same(int size, int count, int stream_count, int shuffle, int gridSize, int blockSize);
