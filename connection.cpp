#include "connection.h"
#include "log/log.h"
#include "hard.h"


C_connection::C_connection(int connection_id):lock(false){
	id = connection_id;
}


C_connection::~C_connection(){};


void C_connection::add_line(C_line& line){
	int n = lines.quantity();
	int i;

	for (i = 0; i < n; i++){	//	 уведомление о добавлении линии
		C_line * l = lines.get(i);
		l->signal(line_signal__line_added);
	}
	
	silent_add_line(line);
}


void C_connection::del_line(C_line& line){
	// удаляет из соединения и отключает на матрице

	silent_del_line(line);


	if (!lock){
		lock = true;
		C_ring_<C_line> lr;
		int n = lines.quantity();
		for (int i = 0; i < n; i++, lines.roll() ){
			 lr.add(lines.get());
		}
		while (lr.quantity()){
			lr.pop()->signal(line_signal__line_deleted);
		}
		lock = false;
	}
	if (!lines.quantity()){
		int n = holded_by.quantity();
		C_ring_<C_line> buf;
		for (int i = 0; i < n; i++){
			buf.add(holded_by.pop());
		}
		free();
		for (int i = 0; i < n; i++){
			C_line * l = buf.pop();
			l->signal(line_signal__holded_freed);	
		}
	}
	Station->del_music(line);
}

void C_connection::silent_add_line(C_line & line){
	lines.add(&line);				//	добавил линию в соединение
	Hard->matrix(id, line.get_id(), on);		//	проключил линию
	line.current_connection = this;			//
}

void C_connection::silent_del_line(C_line & line){	
	int n = lines.quantity();	// количество линий в соединении
	int i;

	for (i = 0; i < n; i++){
		C_line * l = lines.get(i);
		if (*l == line) { //	найти нужную линию 	
			// удалить и отключить
			l->current_connection = NULL;
			Hard->matrix(id, lines.pop(i)->get_id(), off);
			break;
		}
	}
	if (i == n){ 	//	не найдена линия
		Log->set_priority(log_priority__error);
		Log->rec() << "Не найдена линия[" << line.get_id();
		Log->rec() << "] в соединении [" << id << "]";
		Log->write();
		return;	
	}
}

void C_connection::hold(C_line & line){
	int n = lines.quantity();	// количество линий в соединении
	int i;

	for (i = 0; i < n; i++){
		C_line * l = lines.get(i);
		if (*l == line) { //	найти нужную линию 	
			//if (n == 1){
			//	Station->del_music(line);
			//}
			// удалить и отключить
			l->current_connection = NULL;
			l->hold_ring.push(this);			
			Hard->matrix(id, lines.pop(i)->get_id(), off);
			break;
		}
	}
	if (i == n){ 	//	не найдена линия
		Log->set_priority(log_priority__error);
		Log->rec() << "hold: Не найдена линия[" << line.get_id();
		Log->rec() << "] в соединении [" << id << "]";
		Log->write();
		return;	
	}
	for (int n = lines.quantity(), i = 0; i < n; i++, lines.roll()){
		C_line * l = lines.get();
		l->signal(line_signal__holded_by);
	}

	line.switch_stage(line_stage__free);
	holded_by.add(&line);
}

void C_connection::unhold(C_line & line){ // д. вызываться hold_ring.get()->unback(*this);
	if (line.hold_ring.get() != this){
		return;
	}
	line.hold_ring.pop();
	
	int n = holded_by.quantity();
	for (int i = 0; i < n; i++, holded_by.roll()){
		if (holded_by.get() == &line){
			holded_by.pop();
			break;
		}
	}


	line.current_connection = this;
	Hard->matrix(id, line.get_id(), on);
	lines.add(&line);

	for (int n = lines.quantity(), i = 0; i < n; i++, lines.roll()){
		lines.get()->signal(line_signal__unholded_by);
	}
	
	//if (lines.quantity() == 1){
	//	Station->add_music(line);
	//}
}

void C_connection::free(){
	Station->free_connection(this);
}

//	Объединение двух соединений
void C_connection::mix(C_connection& other_connection){
	// перевести все линии другого соединения в текущее
	int n = other_connection.lines.quantity();
	for (int i = 0; i < n; i++){
		C_line * l = other_connection.lines.get();
		Hard->matrix(other_connection.get_id(), l->get_id(), off);
		other_connection.lines.pop();
		lines.add(l);
		Hard->matrix(id, l->get_id(), on);
		l->current_connection = this;
		l->signal(line_signal__unholded_by);
	}
	// освободить другое соединение
	other_connection.free();
	
}

void C_connection::mix_without(C_line& line, C_connection& other_connection){
	// перевести все линии текущего соединения в другое
	int n = other_connection.lines.quantity();
	C_ring_<C_line> lr;
	for (int i = 0; i < n; i++, other_connection.lines.roll()){
		lr.add(other_connection.lines.get());
	}
	n = lines.quantity();
	for (int i = 0; i < n; i++){
		C_line * l = lines.get(i);
		if (*l != line){
			silent_del_line(*l);
			other_connection.silent_add_line(*l);
		}
	}
	while(lr.quantity()){
		lr.pop()->signal(line_signal__unholded_by);
	}
}
