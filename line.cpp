#include "line.h"
#include <stdlib.h>
#include "log/log.h"

int C_line::get_id(){
	return id;
}

C_line::stage_handler C_line::stage_handlers[line_stage_count] = {
	&C_line::stage__free,
	&C_line::stage__ready,
	&C_line::stage__digits,
	&C_line::stage__wait,
	&C_line::stage__retranslation,
	&C_line::stage__connect,
	&C_line::stage__disconnect,
	&C_line::stage__incoming
};

C_line::signal_handler C_line::signal_handlers[line_signal_count] = {
	&C_line::signal__line_deleted,
	&C_line::signal__line_added,
	&C_line::signal__line_dialed,
	&C_line::signal__holded_by,
	&C_line::signal__unholded_by,
	&C_line::signal__holded_freed
};

C_line::C_line(int id_):last_number(NULL), id(id_){
	stage = line_stage__free;
	current_connection = NULL;
	incoming_connection = NULL;
	recognizer = NULL;
	owner_id = -1;
	block_incoming = false;
	block_notify = false;
	blocked = false;
};

C_line::~C_line(){
	if (last_number) delete last_number;
}

void C_line::restore_number(){
	if (!last_number) return;
	if (recognizer) delete recognizer;
	recognizer = new C_pulse_recognizer(last_number);
}

void C_line::remember_number(){
	if (!recognizer) return; 
	if (last_number) delete last_number;
	last_number = *recognizer;

	Log->set_priority(log_priority__info);
	Log->rec() << "линия " << id << " : " << last_number;
	Log->write();
}

int C_line::execute(){
	(this->*stage_handlers[stage])();
	blocked = false;
	return 0;
}

void C_line::signal(line_signal sig){
	(this->*signal_handlers[sig])();
}
