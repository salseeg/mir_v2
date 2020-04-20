#include "outer_line.h"
#include "hard.h"
#include "log/log.h"
#include "busses.h"
	
long line__outer_required_disconnect_timeout = 1500000;// 1.5 sec
unsigned long line__outer_incoming_ring_timeout = 4000000;	// 4 sec
unsigned long line__outer_requried_ring_time = 300000;		// 0.3 sec
int skip_unpowered_count = 7;

C_outer_line::C_outer_line(int id, int priori = 0):C_line(id),
	timer(NULL),
	unpowered(false){

	priority = priori;
}

C_outer_line::~C_outer_line(){
	if (timer) delete timer;
};

void C_outer_line::connect(){
	Hard->set_outer(id, outer_key__ik, on);
	Hard->set_outer(id, outer_key__sk, off);
	Hard->set_outer(id, outer_key__kv, on);
}

void C_outer_line::disconnect(){
	Hard->set_outer(id, outer_key__ik, off);
	Hard->set_outer(id, outer_key__sk, off);
	Hard->set_outer(id, outer_key__kv, off);
}

//***************************
//	signals
//***************
void C_outer_line::signal__holded_freed(){} 	// do not care

void C_outer_line::signal__line_added(){
	if (stage == line_stage__wait){
		switch_stage(line_stage__connect);
		connect();
	}else if (stage == line_stage__retranslation){
		connect();
	}
}

void C_outer_line::signal__line_deleted(){
	if ((stage == line_stage__connect) || (stage == line_stage__retranslation)){
		C_connection * con = current_connection;
		int n = con->lines.quantity();
		int i;
		for (i = 0; i < n; i++, con->lines.roll()){
			if (con->lines.get(i)->active()) break;
		}
		if (i == n){
			switch_stage(line_stage__disconnected);
			disconnect();
			con->del_line(*this);		
		}
	}
}

void C_outer_line::signal__line_dialed(){}	//	do nothin

void C_outer_line::signal__unholded_by(){
	if (current_connection->lines.quantity() > 1)
		Station->del_music(*this);
}

void C_outer_line::signal__holded_by(){
	if (current_connection->lines.quantity() < 2)
		Station->add_music(*this);
}


//**************************
//	stages
//**************
void C_outer_line::stage__incoming(){}		//	never called
	
void C_outer_line::stage__digits(){}		//	never called

void C_outer_line::stage__ready(){}		//	never called

void C_outer_line::stage__connect(){}		//	handled by inner

void C_outer_line::stage__retranslation(){
	if (!Station->switcher.player(id, outer_key__ik).q_length()){
		C_connection * con = current_connection;
		int n = con->lines.quantity();
		for (int i = 0; i < n; i++, con->lines.roll()){
			con->lines.get()->signal(line_signal__line_dialed);
		}
	}
}

void C_outer_line::stage__wait(){
	bit_history &bh = Hard->get_state_outer(id);
	if (bh.flow_state == bit_state__off){
		if (bh.flow_time > line__outer_incoming_ring_timeout){
			C_line * op = NULL;
			int n = Station->lines;
			for (int i = 0; i < n; i++){
				if (Station->lines[i]->incoming_connection == current_connection){
					op = Station->lines[i];
					break;
				}
			}
			if (op){
				op->incoming_connection = NULL;
				Station->switcher.player(op->get_id(), inner_key__pv).reset();
			} else {
				Log->set_priority(log_priority__error);
				Log->rec() << "Line [" << id << "] wait stage : не могу найти оператора!";
				Log->write();
			}
			switch_stage(line_stage__disconnected);
			C_connection * con = current_connection;
			con->del_line(*this);
			//con->free();			
		}else{
			/*//	звоню ли я кому-нибудь
			C_connection * con = current_connection;
			int n = con->lines.quantity();
			if (n < 2){
				C_line * op = NULL;
				int n = Station->lines;
				for (int i = 0; i < n; i++){
					if (Station->lines[i]->incoming_connection == current_connection){
						op = Station->lines[i];
						break;
					}
				}
				if (!op){
					switch_stage(line_stage__disconnected);
					C_connection * con = current_connection;
					con->del_line(*this);
				}
			}*/
		}
	}
}

void C_outer_line::stage__free(){
	static int unpowered_count = 0;
	bit_history &bh = Hard->get_state_outer(id);
	if ((bh.powered == power_error) && (!(bh.flow_state == bit_state__on)) && (unpowered_count++ > skip_unpowered_count)){
		unpowered_count = 0;
		unpowered = true;
		switch_stage(line_stage__disconnected);
		Log->set_priority(log_priority__error);
		Log->rec() << "Линия [" << id << "] : потеря питания!";
		Log->write();
		return;
	}
	if ((bh.flow_state == bit_state__on) && (bh.flow_time > line__outer_requried_ring_time)){
		C_line * op = Busses->get_bus(operator_bus_id)->get_free_line(id, priority);
		C_connection * con = Station->get_free_connection();
		//if (!op && con) cout << con->get_id() << endl;
		if (op && con && !op->incoming_connection){
			con->add_line(*this);
			switch_stage(line_stage__wait);
			op->incoming_connection = con;
			Station->switcher.player(op->get_id(), inner_key__pv).add(melody__ring);
	
			Log->set_priority(log_priority__info);
			Log->rec() << "Внешняя линия (" << id << ") входящее соединение";
			Log->write();
		} else {
			if (con) {
				Station->free_connection(con);
			}
			switch_stage(line_stage__disconnected);
			disconnect();
		}
	}
}

void C_outer_line::stage__disconnect(){
	if (timer){
		if (timer->check()){
			delete timer;
			timer = NULL;	
			if (unpowered){
				bit_history &bh = Hard->get_state_outer(id);
				if (bh.powered == power_ok){
					unpowered = false;

					switch_stage(line_stage__free);
			
					Log->set_priority(log_priority__info);
					Log->rec() << "Линия [" << id << "] : питание востановлено";
					Log->write();
					
				}
			}else{
				switch_stage(line_stage__free);
			}
		}
	}else{
		timer = new C_timer(line__outer_required_disconnect_timeout);
	}
}

int C_outer_line::execute(){
	if (timer){
		timer->execute(Hard->get_delta());
	}
	return C_line::execute();
}
