#include "station.h"
#include "log/log.h"	//	Log

C_station * Station;

C_station::C_station(int n_i_, int n_o_):n_i(n_i_), n_o(n_o_), switcher(n_i_, n_o_){
	music_connection = new C_connection(0);
}

C_station::~C_station(){
	delete music_connection;
}

int C_station::n_in(){
	return n_i;
}

int C_station::n_out(){
	return n_o;
}

//
//	Работа с соединениями
//
C_connection * C_station::get_free_connection(){
	C_connection * res = free_connections.pop();
	if (res){
		connections.add(res);
	}
	return res;	
}

void C_station::free_connection(C_connection * con){
	int n = connections.quantity();

	for (int i = 0; i < n; i++, connections.roll()){					//	??? можно оптимизировать перебор ???
		if (connections.get() == con){
			free_connections.add(connections.pop());
			break;
		}
	}
}

//
// 	Работа с музыкой
//
// 	Музыка на первом шнурке
// 	current_connection указывает на реальное соединение
// 	переключается только матрица
// 	
void C_station::add_music(C_line& ab){
	int i;
	int n = music_connection->lines.quantity();
	for (i = 0; i < n; i++, music_connection->lines.roll()){
		if (*(music_connection->lines.get()) == ab){
			break;
		}
	}
	if (i != n) return;
	C_connection * con = ab.current_connection;
	Hard->matrix(ab.current_connection->get_id(), ab.get_id(), off);
	//Hard->matrix(music_connection->get_id(), ab.get_id(), on);
	music_connection->silent_add_line(ab);
	ab.current_connection = con;
}

void C_station::del_music(C_line& ab){
	int i;
	int n = music_connection->lines.quantity();
	for (i = 0; i < n; i++, music_connection->lines.roll()){
		if (*(music_connection->lines.get()) == ab){
			break;
		}
	}
	if (i == n) return;
	C_connection * con = ab.current_connection;
	music_connection->silent_del_line(ab);
	//Hard->matrix(music_connection->get_id(), ab.get_id(), off);
	ab.current_connection = con;
	if (con){
		Hard->matrix(ab.current_connection->get_id(), ab.get_id(), on);
	}
}

int C_station::execute(){
	int n = lines;
	for (int i = 0; i < n; i++){
		C_line * l = lines[i];
		l->execute();
	}
	switcher.execute(Hard->get_delta());
	return 0;
}
