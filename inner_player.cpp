#include "inner_player.h"

C_inner_player::C_inner_player(int line, inner_key k){
	time = 0;
	line_id = line;
	key = k;
	Hard->set_inner(line_id, key, off);
}

C_inner_player::~C_inner_player(){}

void C_inner_player::finish(){
	Hard->set_inner(line_id, key, off);
	time = 0;
}

void C_inner_player::reset(){
	int n = q.quantity();
	for (int i = 0; i < n; i++)
		q.pop();
	finish();
}

void C_inner_player::add(melody_name name){
	if (q.quantity()){
		if (q.get()->get_looped())
			reset();
	}
	q.add((*Melodies)[name]);
}

void C_inner_player::execute(unsigned long dtime){
	if (!q.quantity()) return;
	time += dtime;
	melody_state ms = q.get()->get_state(time);
	if (ms == melody_state__error){
		q.pop();
		finish();
		execute(0);
	}else{
		on_off st;
		if (ms == melody_state__low){
			st = off;
		}else{
			st = on;
		}
		Hard->set_inner(line_id, key, st);
	}	
}

int C_inner_player::q_length(){
	return q.quantity();
}
