#include "bus.h"
#include <stdlib.h>

C_bus::C_bus(int id, bool roll_){
	bus_id = id;
	roll = roll_;
}

C_bus::~C_bus(){}

C_line * C_bus::get_free_line(int owner_id){
	int n = lines.quantity();
	if (owner_id >= 0){
		for (int i = 0; i < n; i++, lines.roll()){
			C_line * l = lines.get();
			if ((l->stage == line_stage__free) && (l->owner_id == owner_id) && !(l->blocked)){
				if (!l->block_incoming){
					if (!roll) lines.roll(-i);
					l->blocked = true;
					return l;
				}
			}
		}
	}
	for (int i = 0; i < n; i++, lines.roll()){
		C_line * l = lines.get();
		if ((l->stage == line_stage__free) && !(l->blocked)){
			//if (l->owner_id < 0){
			if (!l->block_incoming){
				if (!roll) lines.roll(-i);
				l->blocked = true;
				return l;
			}
			//}
		}
	}
	return NULL;
}

int C_bus::get_free_line_id(int owner_id){
	C_line * l = get_free_line(owner_id);
	if (l){
		return l->get_id();
	}
	return -1;
}

int C_bus::get_bus_id(){
	return bus_id;
}
