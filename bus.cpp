#include "bus.h"
#include <stdlib.h>

C_bus::C_bus(int id, bool roll_){
	bus_id = id;
	roll = roll_;
}

C_bus::~C_bus(){}

C_line * C_bus::get_free_line(int owner_id, int priority){
	C_line * r = NULL;
	r = get_free_reserved(owner_id);
	if (!r) {
		r = get_free();
	}
	if (!r) {
		r = steal_by_priority(priority);
	}
	return r;
}

C_line * C_bus::get_free_reserved(int owner_id){
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
	return NULL;
}

C_line * C_bus::get_free(){
	int n = lines.quantity();
	for (int i = 0; i < n; i++, lines.roll()){
		C_line * l = lines.get();
		if ((l->stage == line_stage__free) && !(l->blocked)){
			if (!l->block_incoming){
				if (!roll) lines.roll(-i);
				l->blocked = true;
				return l;
			}
		}
	}
	return NULL;
}

C_line * C_bus::steal_by_priority(int priority){
	C_line * min_priority_connection_line = NULL; 
	int n = lines.quantity();
	for (int i = 0; i < n; i++, lines.roll()){
		C_line * l = lines.get();
		if (!min_priority_connection_line){
			min_priority_connection_line = l;
			continue;
		}
		if (min_priority_connection_line->current_connection->get_priority() > l->current_connection->get_priority()){
			min_priority_connection_line = l;
		}		
	}
	if (min_priority_connection_line->current_connection->get_priority() < priority){
		min_priority_connection_line->current_connection->steal_line(min_priority_connection_line);
		min_priority_connection_line->switch_stage(line_stage__free);
		min_priority_connection_line->blocked = true;
		return min_priority_connection_line;
	}else{
		return NULL;
	}
}

int C_bus::get_free_line_id(int owner_id, int prio){
	C_line * l = get_free_line(owner_id, prio);
	if (l){
		return l->get_id();
	}
	return -1;
}

int C_bus::get_bus_id(){
	return bus_id;
}
