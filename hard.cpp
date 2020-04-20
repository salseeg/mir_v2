#include "hard.h"
#include "log/log.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

C_hard * Hard = NULL; 

C_hard::C_hard(char * fn, int n_i, int n_o){
	driver_descriptor = ::open(fn, O_RDWR);
	if (driver_descriptor < 0){
		Log->set_priority(log_priority__alert);
		Log->rec() << "Не могу открыть " << fn;
		Log->write();
		exit(-1);
	}
	
	n_inner = n_i;
	n_outer = n_o;

	kna_m = false;
	pv_m = false;
	kv_m = false;
	sk_m = false;
	ik_m = false;

	kna = new on_off[n_inner];
	pv  = new on_off[n_inner];
	kv  = new on_off[n_outer];
	sk  = new on_off[n_outer];
	ik  = new on_off[n_outer];
	
	inner_states = new bit_history [n_inner << 2];
	outer_states = new bit_history [n_outer << 2];

	write_command wc;
	wc.type = CT_init;
	if (::write(driver_descriptor, &wc, sizeof(wc)) < 0){
		Log->set_priority(log_priority__error);
		Log->rec() << "CT_init";
		Log->write();
	}
	
	read();
}

C_hard::~C_hard(){
	delete kna;
	delete pv;
	delete kv;
	delete sk;
	delete ik;
	delete inner_states;
	delete outer_states;
	::close(driver_descriptor);
}


void C_hard::read(){
	read_line_states rs;

	rs.bits = inner_states;
	rs.line_type = LT_inner;
	rs.quantity = n_inner;
	
	if (::read(driver_descriptor, &rs, sizeof(rs)) < 0){
		Log->set_priority(log_priority__error);
		Log->rec() << "read_inner " << strerror(errno) << errno;
		Log->write();
	}
	delta = rs.delta_time;
	
	rs.bits = outer_states;
	rs.line_type = LT_outer;
	rs.quantity = n_outer;
	if (::read(driver_descriptor, &rs, sizeof(rs))	< 0){
		Log->set_priority(log_priority__error);
		Log->rec() << "read_outer " << strerror(errno) << errno;
		Log->write();
	}
}

unsigned long C_hard::get_delta(){
	return delta;
}

void C_hard::sleep_us(){
	if (kna_m){
		write_command wc;
		wc.type = CT_KNA;		
		for (int i = 0; i < n_inner; i++){
			wc.line_number = i;
			wc.state = kna[i];
			::write(driver_descriptor, &wc, sizeof(wc));
		}
		kna_m = false;
	}

	if (pv_m){			
		write_command wc;
		wc.type = CT_PV;
		for (int i = 0; i < n_inner; i++){
			wc.line_number = i;
			wc.state = pv[i];
			::write(driver_descriptor, &wc, sizeof(wc));
		}
		pv_m = false;
	}

	if (kv_m){
		write_command wc;
		wc.type = CT_KV;
		for (int i = 0; i < n_outer; i++){
			wc.line_number = i;
			wc.state = kv[i];
			::write(driver_descriptor, &wc, sizeof(wc));
		}
		kv_m = false;
	}

	if (sk_m){
		write_command wc;
		wc.type = CT_SK;
		for (int i = 0; i < n_outer; i++){
			wc.line_number = i;
			wc.state = sk[i];
			::write(driver_descriptor, &wc, sizeof(wc));
		}
		sk_m = false;
	}

	if (ik_m){
		write_command wc;
		wc.type = CT_IK;
		for (int i = 0; i < n_outer; i++){
			wc.line_number = i;
			wc.state = ik[i];
			::write(driver_descriptor, &wc, sizeof(wc));
		}
		ik_m = false;
	}
	
	write_command wc;
	wc.type = CT_sleep;
	::write(driver_descriptor, &wc, sizeof(wc));
	
	read();
}

bit_history & C_hard::get_state_inner(int inner_line){
	return inner_states[inner_line];
}

bit_history & C_hard::get_state_outer(int outer_line){
	outer_line -= n_inner;		//	matrix2outer convertion
	return outer_states[outer_line];
}

void C_hard::matrix(int cord, int matrix_line, on_off st){
	write_command wc = {CT_matrix, cord, matrix_line, st};
	::write(driver_descriptor, &wc, sizeof(wc));
	
#ifdef LOG_MATRIX
	Log->set_priority(log_priority__debug);
	Log->rec() << "matrix " << cord << "x" << matrix_line;
	Log->rec() << (st ? " on": " off");
	Log->write();
#endif
}

void C_hard::set_inner(int inner_line, inner_key key, on_off st){
	switch (key){
		case inner_key__kna:{
			kna[inner_line] = st;
			kna_m = true;
			break;
		}
		case inner_key__pv:{
			pv[inner_line] = st;
			pv_m = true;
			break;
		}
	}
}

void C_hard::set_outer(int outer_line, outer_key key, on_off st){
	outer_line -= n_inner;		//	matrix2outer convertion
	switch (key){
		case outer_key__ik:{
			ik[outer_line] = st;
			ik_m = true;
			break;
		}
		case outer_key__sk:{
			sk[outer_line] = st;
			sk_m = true;
			break;
		}
		case outer_key__kv:{
			kv[outer_line] = st;
			kv_m = true;
			break;
		}
	}
}
