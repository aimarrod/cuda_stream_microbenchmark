#include <stdlib.h>

int *generate_sequence(int count, bool shuffled){
	int *array = new int[count];
	for(int i = 0; i < count; i++){
		array[i] = i;	
	}

	if (shuffled && count > 1){
		for (int i = 0; i < count - 1; i++) 
		{
			int j = i + rand() / (RAND_MAX / (count - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}

	return array;
}

bool *generate_flags(int count){
	bool *flags = new bool[count];
	for(int i = 0; i < count; ++i){
		flags[i] = false;
	}

	return flags;
}

void free_sequence(int *objects){	
	delete[] objects;
}

void free_flags(bool *flags){
	delete[] flags;
}
