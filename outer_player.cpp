#include <mir/outer_player.h>

C_outer_player::C_outer_player(int line, outer_key k){
	time = 0;
	line_id = line;
	key = k;
}

C_outer_player::~C_outer_player(){}

void C_outer_player::finish(){
	if (key == outer_key__ik){
		Hard->set_outer(line_id, outer_key__sk, off);
		Hard->set_outer(line_id, outer_key__kv, on);
		Hard->set_outer(line_id, outer_key__ik, on);
	}else{
		Hard->set_outer(line_id, key, off);
	}
	time = 0;
}

void C_outer_player::reset(){
	int n = q.quantity();
	for (int i = 0; i < n; i++)
		q.pop();
	finish();
}

void C_outer_player::add(melody_name name){
	if (q.quantity()){
		if (q.get()->get_looped())
			reset();
	}
	q.add((*Melodies)[name]);
	if (key == outer_key__ik){
		Hard->set_outer(line_id, outer_key__sk, on);
		Hard->set_outer(line_id, outer_key__kv, on);
		Hard->set_outer(line_id, outer_key__ik, on);
	}
}

void C_outer_player::execute(unsigned long dtime){
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
		if (key == outer_key__ik){
			st = (st == on)? off : on;
		}
		Hard->set_outer(line_id, key, st);
	}	
}

int C_outer_player::q_length(){
	return q.quantity();
}
