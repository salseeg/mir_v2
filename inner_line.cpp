#include "inner_line.h"
#include "hard.h"
#include "log/log.h"
#include "masks.h"
#include "connection_masks.h"

unsigned long line__ready_timeout = 20000000;		//	20 sec
unsigned long line__required_disconnet_time = 200000;	//	0.2 sec
unsigned long line__wait_4_connect_timeout = 45000000;	//	45 sec

C_inner_line::C_inner_line(int id_, int priori = 0):C_line(id_){
	priority = priori;
}

C_inner_line::~C_inner_line(){};

bool C_inner_line::active(){
    return true;
}

///////////////////////////////////
//	Сигналы
////////////////	
void C_inner_line::signal__holded_freed(){
	int n = hold_ring.quantity();
	for (int i = 0; i < n; i++, hold_ring.roll()){
		C_connection * con = hold_ring.get();
		if (!con->holded_by.quantity()){
			hold_ring.pop();
			hold_ring.roll(-i);		
		}
	}
}

void C_inner_line::signal__line_added(){
	if (stage == line_stage__wait){
		switch_stage(line_stage__connect);
		Station->switcher.player(id, inner_key__kna).reset();
		if (!recognizer){
			recognizer = new C_pulse_recognizer;
		}
	}
}

void C_inner_line::signal__line_deleted(){
	if (stage == line_stage__connect){
		if (current_connection->lines.quantity() < 2){
			switch_stage(line_stage__disconnected);
			current_connection->del_line(*this);
			Station->switcher.player(id, inner_key__kna).add(melody__busy);	
			if (recognizer) {
				delete recognizer;
				recognizer = NULL;
			}
		}
	}
}

void C_inner_line::signal__line_dialed(){
	if (stage == line_stage__wait){
		switch_stage(line_stage__connect);
		Station->switcher.player(id, inner_key__kna).reset();
		if (recognizer) delete recognizer;
		recognizer = new C_pulse_recognizer;
		C_connection * con = current_connection;
		for (int i = 0, n = con->lines.quantity(); i < n; i++, con->lines.roll()){
			C_line * b_line = con->lines.get();
			if (*b_line != *this){
				b_line->switch_stage(line_stage__connect);
				break;
			}
		}
	}
}

void C_inner_line::signal__unholded_by(){
	if (current_connection->lines.quantity() > 1)
		Station->del_music(*this);
		if (!recognizer){
			recognizer = new C_pulse_recognizer;
		}
}

void C_inner_line::signal__holded_by(){
	if (current_connection->lines.quantity() < 2){
		Station->add_music(*this);
		if (recognizer){
			delete recognizer;
			recognizer = NULL;
		}
	}
}

///////////////////////////////////
//	Состояния
//////////////////
void C_inner_line::stage__disconnect(){
	bit_history &bh = Hard->get_state_inner(id);
	if (bh.flow_state == bit_state__off){
		if (bh.flow_time > line__required_disconnet_time){
			switch_stage(line_stage__free);
			Station->switcher.player(id, inner_key__kna).reset();
			Station->switcher.player(id, inner_key__pv).reset();
		}
	}
}


void C_inner_line::stage__ready(){
	bit_history &bh = Hard->get_state_inner(id);
	if (bh.flow_state == bit_state__off){
		switch_stage(line_stage__digits);
		Station->switcher.player(id, inner_key__kna).reset();
		recognizer = new C_pulse_recognizer();
	}else{
		if (bh.flow_time > line__ready_timeout){
			switch_stage(line_stage__disconnected);
			Station->switcher.player(id, inner_key__kna).add(melody__busy);
			C_connection * con = current_connection;
			con->del_line(*this);
			// con->free();
		}
	}
}

void C_inner_line::stage__digits(){
	bit_history &bh = Hard->get_state_inner(id);
	recognition_result rr =	recognizer->recognize(bh);
	if (bh.flow_time > 60000000){
		rr = recognition_result__error;
	}
	switch (rr){
		case recognition_result__error:{
			switch_stage(line_stage__disconnected);
			Station->switcher.player(id, inner_key__kna).add(melody__busy);
			C_connection * con = current_connection;
			con->del_line(*this);
			delete recognizer;
			recognizer = NULL;
		}break;
		case recognition_result__digit:{
			Masks->check(id);
		}break;
		default:
			break;
	}
}

void C_inner_line::stage__wait(){
	bit_history &bh = Hard->get_state_inner(id);
	if ((bh.flow_state == bit_state__off) || 
	    (bh.flow_time > line__wait_4_connect_timeout)){
		int i;
		int n = Station->lines;
		for (i = 0; i < n; i++){
			C_line * l  = Station->lines[i];
			if (*l != *this){
				if (l->incoming_connection == current_connection){
					l->incoming_connection = NULL;
					Station->switcher.player(l->get_id(), inner_key__pv).reset();
					Station->switcher.player(l->get_id(), inner_key__kna).reset();
					break;
				}
			}
		}

		if (i == n){	
			//	 error !!!! TODO найти причину 
			Log->set_priority(log_priority__error);
			Log->rec() << "Line[" << id << "] wait stage : не найдена вызываемая линия !";
			Log->write();
		}

		C_connection * con = current_connection;
		con->del_line(*this);
		//con->free();

		Station->switcher.player(id, inner_key__kna).add(melody__busy);
		switch_stage(line_stage__disconnected);
		if (recognizer) {
			delete recognizer;
			recognizer = NULL;
		}
	}
}	

void C_inner_line::stage__connect(){
	bit_history & bh = Hard->get_state_inner(id);
	if (recognizer){
		recognition_result rr = recognizer->recognize(bh);
		switch (rr){
			case recognition_result__digit:{
				Connection_masks->check(id);
			}break;
			case recognition_result__error:{
				switch_stage(line_stage__disconnected);
				current_connection->del_line(*this);
				Station->switcher.player(id, inner_key__kna).add(melody__busy);
				if (recognizer) {
					delete recognizer;
					recognizer = NULL;
				}
			}break;
			default:break;
		}
	}else{
		if (bh.flow_state == bit_state__off){
			switch_stage(line_stage__disconnected);
			current_connection->del_line(*this);
			Station->switcher.player(id, inner_key__kna).add(melody__busy);
			if (recognizer) {
				delete recognizer;
				recognizer = NULL;
			}
		}
	}
}

void C_inner_line::stage__retranslation(){
	bit_history & bh = Hard->get_state_inner(id);
	recognition_result rr = recognizer->recognize(bh);
	C_line * b_line = this;
	while (*b_line == *this){
		b_line = current_connection->lines.get();
		current_connection->lines.roll();
	}
	switch(rr){
		case recognition_result__error:{
			switch_stage(line_stage__disconnected);
			current_connection->del_line(*this);
			Station->switcher.player(id, inner_key__kna).add(melody__busy);
			if (recognizer) {
				delete recognizer;
				recognizer = NULL;
			}			
		}break;
		case recognition_result__not_started:{}break;
		case recognition_result__not_ended:{
			int b_id = b_line->get_id();
			Hard->set_outer(b_id, outer_key__kv, on);
			Hard->set_outer(b_id, outer_key__sk, on);
			Hard->set_outer(b_id, outer_key__ik, (on_off) bh.flow_state);
		}break;
		case recognition_result__digit:{
			int b_id = b_line->get_id();
			Hard->set_outer(b_id, outer_key__kv, on);
			Hard->set_outer(b_id, outer_key__sk, off);
			Hard->set_outer(b_id, outer_key__ik, off);
		}break;
	}
}

void C_inner_line::stage__incoming(){
	Log->set_priority(log_priority__error);
	Log->rec() << "Line[" << id << "] incoming stage : недопустимое состояние!";
	Log->write();
	switch_stage(line_stage__free);
}

void C_inner_line::stage__free(){
	bit_history &bh = Hard->get_state_inner(id);

	if (bh.flow_state == bit_state__on){
		if (incoming_connection){
			incoming_connection->add_line(*this);
			incoming_connection = NULL;
			switch_stage(line_stage__connect);
			recognizer = new C_pulse_recognizer;
			Station->switcher.player(id, inner_key__kna).reset();	
			Station->switcher.player(id, inner_key__pv).reset();
		}else if (hold_ring.quantity()){
			hold_ring.get()->unhold(*this);
			switch_stage(line_stage__connect);
			recognizer = new C_pulse_recognizer;
			Station->switcher.player(id, inner_key__kna).reset();	
			Station->switcher.player(id, inner_key__pv).reset();
		}else{
			current_connection = Station->get_free_connection();
			if (current_connection){
				current_connection->add_line(*this);
				switch_stage(line_stage__ready);
				Station->switcher.player(id, inner_key__kna).add(melody__ready);
			}else{
				switch_stage(line_stage__disconnected);
				Station->switcher.player(id, inner_key__kna).add(melody__deny);
			}
		}
	}else{
		if (incoming_connection){
			C_player & pl = Station->switcher.player(id, inner_key__pv);
			if (pl.q_length() == 0){
				
				pl.add(melody__ring);
				
				int n = incoming_connection->lines.quantity();
				for (int i = 0; i < n; i++, incoming_connection->lines.roll()){
					C_line * b = incoming_connection->lines.get();
					if (*b != *this){
						Station->switcher.player(b->get_id(), inner_key__kna).add(melody__ring_control);
						break;
					}
				}
			}		
		} else if (hold_ring.quantity()){
			C_player & pl = Station->switcher.player(id, inner_key__pv);
			if (pl.q_length() == 0){
				pl.add(melody__ring);
			}
		}
	}
}
