#include "connection_masks.h"
#include "station.h"
#include "busses.h"

C_connection_masks * Connection_masks = NULL;

C_connection_masks::action C_connection_masks::actions[connection_mask_action_count] = {
	&C_connection_masks::action__hold,
	&C_connection_masks::action__mix,
	&C_connection_masks::action__mix_without,
	&C_connection_masks::action__auto_hold	
};

void C_connection_masks::action__hold(int line_id, int){
	C_line * int_line = Station->lines[line_id];
	int_line->current_connection->hold(*int_line);
	C_connection * con = Station->get_free_connection();
	if (con){
		int_line->switch_stage(line_stage__ready);
		int_line->current_connection = con;
		con->add_line(*int_line);
		Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__ok);
		Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__ready);
	}else{
		int_line->switch_stage(line_stage__disconnected);
	}	
	if (int_line->recognizer) {
		delete int_line->recognizer;
		int_line->recognizer = NULL;
	}
}

void C_connection_masks::action__mix(int line_id, int){
	C_line * int_line = Station->lines[line_id];
	if (int_line->hold_ring.quantity()){
		C_connection * oth_con = int_line->hold_ring.pop();
		int_line->current_connection->mix(*oth_con);
	}
	if (int_line->recognizer) {
		delete int_line->recognizer;
		int_line->recognizer = new C_pulse_recognizer;
	}
}	

void C_connection_masks::action__mix_without(int line_id, int){
	C_line * int_line = Station->lines[line_id];
	if (int_line->hold_ring.quantity()){
		C_connection * oth_con = int_line->hold_ring.pop();
		int_line->current_connection->mix_without(*int_line, *oth_con);
		
		//C_connection * con = Station->get_free_connection();
		C_connection * con = int_line->current_connection;
		if (con){
			int_line->switch_stage(line_stage__ready);
			//int_line->current_connection = con;
			Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__ok);
			Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__ready);
		}else{
			int_line->switch_stage(line_stage__disconnected);
			Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__busy);
		}		
		if (int_line->recognizer) {
			delete int_line->recognizer;
			int_line->recognizer = NULL;
		}
	}else{
		if (int_line->recognizer){
			delete int_line->recognizer;
			int_line->recognizer = new C_pulse_recognizer;
		}
	}
}

void C_connection_masks::action__auto_hold(int line_id,int arg){
	C_line * int_line = Station->lines[line_id];
	int_line->current_connection->hold(*int_line);
	C_connection * con = Station->get_free_connection();
	C_line * b_line = Busses->get_bus(arg)->get_free_line(line_id);
	if (con && b_line && !b_line->incoming_connection){
		int_line->current_connection = con;
		con->add_line(*int_line);
		b_line->incoming_connection = con;

		int_line->switch_stage(line_stage__wait);		
		
		Station->switcher.player(line_id, inner_key__kna).add(melody__ring);
		Station->switcher.player(b_line->get_id(), inner_key__pv).add(melody__ring);
	}else{
		Station->switcher.player(line_id, inner_key__kna).add(melody__busy);
		int_line->switch_stage(line_stage__disconnected);
		if (con) Station->free_connection(con);
	}	
	if (int_line->recognizer) {
		delete int_line->recognizer;
		int_line->recognizer = NULL;
	}
}

void C_connection_masks::check(int line_id){
	int n = ring.quantity();	
	bool not_match = true;
	for (int i = 0; i < n; i++, ring.roll(1)){
		C_connection_mask * m = ring.get();
		connection_mask_check cr = m->check(line_id);
		if (cr == connection_mask_check__not_match) continue;
		if (cr == connection_mask_check__not_allowed){
			ring.roll(-i);
			break;
		}
		if (cr == connection_mask_check__completed){
			ring.roll(-i);
			action f = actions[m->func];
			(this->*f)(line_id,m->arg);
			return;
		}
		not_match = false;
	}
	if (not_match){
		// do nothing 
		
		C_line * int_line = Station->lines[line_id];
		if (int_line->recognizer){
			delete int_line->recognizer;
			int_line->recognizer = new C_pulse_recognizer;
		}		
	}	
}	
