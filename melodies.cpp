#include <stdlib.h>		//	NULL
#include <mir/melodies.h>

C_melodies * Melodies = NULL;

C_melodies::C_melodies(int count){
	array = count;
	for (int i = 0; i < count; i++){
		array[i] = NULL;
	}
}

C_melodies::~C_melodies(){
	int n = array;
	for (int i = 0; i < n; i++){
		if (array[i]) 
			delete array[i];
	}
}

C_melody*& C_melodies::operator[](melody_name n){
	return array[(int) n];	
}
