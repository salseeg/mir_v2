#include "masks.h"
#include "station.h"
#include "switcher.h"
#include "busses.h"
//#include "hard.h"

C_masks * Masks = NULL;

C_masks::action C_masks::actions[mask_action_count] = {
	&C_masks::action__connect,
	&C_masks::action__connect_local,
	&C_masks::action__redial,
	&C_masks::action__redirect,	
	&C_masks::action__reserve,	
	&C_masks::action__direct_connect,	
	&C_masks::action__block_incoming,
	&C_masks::action__block_notify	
};

//=================================================
//
//	Actions
//
//================================================	


void C_masks::action__connect(int line_id, int bus_id, int skip_len){
	if (retranslate_recognized(line_id, bus_id, skip_len)){
		C_line * int_line = Station->lines[line_id];
		int_line->switch_stage(line_stage__wait);
		int_line->remember_number();
		delete int_line->recognizer;
		int_line->recognizer = NULL;
	}else{
		C_line * int_line = Station->lines[line_id];
		int_line->switch_stage(line_stage__disconnected);
		Station->switcher.player(line_id, inner_key__kna).add(melody__busy);
		C_connection * con = int_line->current_connection;
		if (con){
			con->del_line(*int_line);
		}
		if (int_line->recognizer){
			delete int_line->recognizer;
			int_line->recognizer = NULL;
		}		
	}
}

void C_masks::action__connect_local(int line_id, int bus_id, int){
	C_line * a_line = Station->lines[line_id];
	C_bus * b_bus = Busses->get_bus(bus_id);
	C_line * b_line = b_bus->get_free_line(line_id, a_line->priority);
	C_connection * con = a_line->current_connection;
	
	// con->add_line(*a_line);
	a_line->switch_stage(line_stage__wait);
	
	if (b_line){	//	линия Б свободна
		if (b_line->incoming_connection){
			a_line->switch_stage(line_stage__disconnected);
			con->del_line(*a_line);
			Station->switcher.player(a_line->get_id(), inner_key__kna).add(melody__busy);
			if (a_line->recognizer){
				delete a_line->recognizer;
				a_line->recognizer = NULL;
			}
		}else{
			b_line->incoming_connection = con;
			Station->switcher.player(a_line->get_id(), inner_key__kna).add(melody__ring_control);
			Station->switcher.player(b_line->get_id(), inner_key__pv).add(melody__ring);
			if (a_line->recognizer){
				delete a_line->recognizer;
				a_line->recognizer = new C_pulse_recognizer;
			}
		}
	}else{		//	линия Б занята
		b_line = b_bus->lines.get();
		b_bus->lines.roll();
		if (b_line->incoming_connection){
			a_line->switch_stage(line_stage__disconnected);
			con->del_line(*a_line);
			Station->switcher.player(a_line->get_id(), inner_key__kna).add(melody__busy);
			if (a_line->recognizer){
				delete a_line->recognizer;
				a_line->recognizer = NULL;				
			}
		}else{
			if (b_line->stage == line_stage__connect && !b_line->block_notify){
				b_line->incoming_connection = con;
				Station->switcher.player(a_line->get_id(), inner_key__kna).add(melody__notify);
				Station->switcher.player(b_line->get_id(), inner_key__kna).add(melody__notify);			
				if (a_line->recognizer){
					delete a_line->recognizer;
					a_line->recognizer = new C_pulse_recognizer;
				}
			}else{
				a_line->switch_stage(line_stage__disconnected);
				con->del_line(*a_line);
				Station->switcher.player(a_line->get_id(), inner_key__kna).add(melody__busy);
				if (a_line->recognizer){
					delete a_line->recognizer;
					a_line->recognizer = NULL;				
				}
			}
		}		
	}
}

void C_masks::action__redial(int line_id, int, int){
	Station->lines[line_id]->restore_number();
}

void C_masks::action__redirect(int line_id, int, int skip_len){
	C_line * int_line = Station->lines[line_id];
	bool success = false;
	C_pulse_recognizer& pr = *int_line->recognizer;
	int n = pr.get_length() - skip_len;
	switch (n){
		case 2:{
			if (pr[skip_len] == 1){
				if (pr[skip_len + 1] == 1) break;
				if (pr[skip_len + 1] == 0){
					// make default
					for (int i = Station->n_in(), n = Station->n_in() + Station->n_out(); i < n; i++){
						Station->lines[i]->operator_bus_id = 101;	//	assume 101 - operator bus
						success = true;
					}
				} else { // assume [2-9]
					int bus_id = Station->lines[pr[skip_len + 1] - 2]->own_bus_id;
					for (int i = 0; i < 12 ; i++){
						Station->lines[i]->operator_bus_id = bus_id;
					}
					success = true;
				}
			} else if ((pr[skip_len] < 2) || (pr[skip_len] > 9)){
				break;
			}else{
				// all to each
				
				Station->lines[pr[skip_len] - 2]->operator_bus_id = Station->lines[pr[skip_len + 1] - 2]->own_bus_id;
				success = true;
			}		
		}break;
		case 3:{
			if (pr[skip_len]) break;
			if ((pr[skip_len + 1] < 1) || (pr[skip_len + 1]) > 4) break;
			if ((pr[skip_len + 2] < 2) || (pr[skip_len + 2]) > 9) break;
			
			// assume разница между номером и ид = 2
			Station->lines[pr[skip_len + 1] + Station->n_in()]->operator_bus_id = Station->lines[pr[skip_len + 2] - 2]->own_bus_id;	
			
			success = true;
		}break;
	}	
	int_line->current_connection->del_line(*int_line);
	int_line->switch_stage(line_stage__disconnected);
	Station->switcher.player(int_line->get_id(), inner_key__kna).add(success ? melody__ok : melody__deny);	

	delete int_line->recognizer;
	int_line->recognizer = NULL;
}

void C_masks::action__reserve(int line_id, int, int skip_len){
	C_line * int_line = Station->lines[line_id];
	bool success = false;
	C_pulse_recognizer& pr = *int_line->recognizer;
	if (pr[skip_len] == 0){
		for (int i = Station->n_in(), n = Station->n_in() + Station->n_out(); i < n; i++){
			C_line * l = Station->lines[i];
			if (l->owner_id == line_id){
				l->owner_id = -1;
			}
		}
		success = true;
	}else{
		if (pr[skip_len] <= Station->n_out()){
			if (Station->lines[Station->n_in() + pr[skip_len] - 1]->owner_id == -1){
				Station->lines[Station->n_in() + pr[skip_len] - 1]->owner_id = line_id;
				success = true;
			}
		}
	}
	int_line->current_connection->del_line(*int_line);
	int_line->switch_stage(line_stage__disconnected);
	Station->switcher.player(int_line->get_id(), inner_key__kna).add(success ? melody__ok : melody__deny);	

	delete int_line->recognizer;
	int_line->recognizer = NULL;
}

void C_masks::action__direct_connect(int line_id, int bus_id, int){
	C_line * int_line = Station->lines[line_id];
	C_line * b_line = Busses->get_bus(bus_id)->get_free_line(line_id, int_line->priority);
	if (b_line){
		int_line->current_connection->silent_add_line(*b_line);
		b_line->switch_stage(line_stage__connect);
		int_line->switch_stage(line_stage__retranslation);

		delete int_line->recognizer;
		int_line->recognizer = new C_pulse_recognizer();
		Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__ok);	
	}else{
		int_line->current_connection->del_line(*int_line);
		int_line->switch_stage(line_stage__disconnected);
		Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__deny);	

		delete int_line->recognizer;
		int_line->recognizer = NULL;
	}
}


void C_masks::action__block_incoming(int line_id, int, int skip_len){
	//	block_incoming = pr[skip_len]
	C_line * int_line = Station->lines[line_id];
	C_pulse_recognizer& pr = *int_line->recognizer;
	
	int_line->block_incoming = (pr[skip_len] != 0);
	
	int_line->current_connection->del_line(*int_line);
	int_line->switch_stage(line_stage__disconnected);
	Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__ok);	

	delete int_line->recognizer;
	int_line->recognizer = NULL;
}

void C_masks::action__block_notify(int line_id, int, int skip_len){
	//	block_incoming = pr[skip_len]
	C_line * int_line = Station->lines[line_id];
	C_pulse_recognizer& pr = *int_line->recognizer;
	
	int_line->block_notify = (pr[skip_len] != 0);
	
	int_line->current_connection->del_line(*int_line);
	int_line->switch_stage(line_stage__disconnected);
	Station->switcher.player(int_line->get_id(), inner_key__kna).add(melody__ok);	

	delete int_line->recognizer;
	int_line->recognizer = NULL;
}

//======================================================
//
//	check
//
//======================================================

enum retr_state {
	zero,
	one,
	bus_mismatch
};

void C_masks::check(int line_id){
	int n = ring.quantity();
	retr_state st = zero;
	int bus_id = -1;
	int skip_len = -1;
	bool not_match = true;
	for (int i = 0; i < n; i++){
		C_mask * m = ring.get(i);
		mask_check cr = m->check(line_id);
		if (cr == mask_check__not_match) continue;
		if (cr == mask_check__not_allowed){
			//ring.roll(-i);
			break;
		}
		if ((cr == mask_check__completed) && not_match){
			//ring.roll(-i);
			action f = actions[m->func];
			(this->*f)(line_id,m->bus_id, m->skip_len);
			return;
		}
		not_match = false;
		if (m->func != mask_action__connect) continue;
		// если звонок наружу
		if (st == zero){
			bus_id = m->bus_id;
			skip_len = m->skip_len;
			st = one;
		}else if (st == one){
			if ((bus_id != m->bus_id) || (skip_len != m->skip_len))
				st = bus_mismatch;
		}		
	}
	if (st == one){
		if (!retranslate_recognized(line_id, bus_id, skip_len)){
			not_match = true;
		}
	}
	if (not_match){
		// disconnect --(signal__line_deleted)--> ext_disconnect
		
		C_line * int_line = Station->lines[line_id];
		int_line->switch_stage(line_stage__disconnected);
		Station->switcher.player(line_id, inner_key__kna).add(melody__deny);
		C_connection * con = int_line->current_connection;
		if (con){
			con->del_line(*int_line);
			//con->free();
		}
		if (int_line->recognizer){
			delete int_line->recognizer;
			int_line->recognizer = NULL;
		}		
	}	
}	

bool C_masks::retranslate_recognized(int line_id, int bus_id, int skip_len){
	C_line * int_line = Station->lines[line_id];
	C_connection * con = int_line->current_connection;
	C_line * ext_line = NULL;
	if (con->lines.quantity() == 2){	// продолжаем
		ext_line = int_line;
		while (*ext_line == *int_line){
			ext_line = con->lines.get();
			con->lines.roll(1);
		}			
	}else{	// начинаем
		C_bus * bus = Busses->get_bus(bus_id);
		ext_line = bus->get_free_line(line_id, int_line->priority);
		if (!ext_line) return false;
		con->silent_add_line(*ext_line);
		ext_line->switch_stage(line_stage__retranslation);
		ext_line->signal(line_signal__line_added);		// to connect outer ????
		Station->switcher.player(ext_line->get_id(), outer_key__ik).add(melody__initial_pause);
	}
	int ret_count = int_line->recognizer->retranslated_count;
	int num_length = int_line->recognizer->get_length();
	for (int i = skip_len + ret_count; i < num_length; i++){
		Station->switcher.player(ext_line->get_id(), outer_key__ik).add((melody_name)(*(int_line->recognizer))[i]);
		int_line->recognizer->retranslated_count++;
	}
	return true;
}


