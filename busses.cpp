#include "busses.h"
#include <stdlib.h>

C_busses * Busses = NULL;

C_bus * C_busses::get_bus(int bus_id){
	int n = ring.quantity();
	for (int i = 0; i < n; i++){
		C_bus * b = ring.get(i);
		if (b->get_bus_id() == bus_id){
			ring.pop(i);
			ring.push(b);
			return b;
		}
	}
	return NULL;	
}

C_busses::~C_busses(){
	int n = ring.quantity();
	while (n--){
		delete ring.pop();
	}
}
