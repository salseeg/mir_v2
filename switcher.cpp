#include "switcher.h"
#include "inner_player.h"
#include "outer_player.h"

C_switcher::C_switcher(int n_i, int n_o){
	n_inner = n_i;
	n_outer = n_o;
	
	inner = new C_player** [n_inner];
	for (int i = 0; i < n_inner; i++){
		inner[i] = new C_player* [inner_key_count];
		for (int j = 0; j < inner_key_count; j++){
			inner[i][j] = new C_inner_player(i, (inner_key) j);
		}
	}
	
	outer = new C_player** [n_outer];
	for (int i = 0; i < n_outer; i++){
		outer[i] = new C_player* [outer_key_count];
		for (int j = 0; j < outer_key_count; j++){
			outer[i][j] = new C_outer_player(i + n_inner, (outer_key) j);
		}
	}	
}

C_switcher::~C_switcher(){
	for (int i = 0; i < n_inner; i++){
		for (int j = 0; j < inner_key_count; j++){
			delete inner[i][j];
		}
		delete[] inner[i];
	}
	delete[] inner;
	
	for (int i = 0; i < n_outer; i++){
		for (int j = 0; j < outer_key_count; j++){
			delete outer[i][j];
		}
		delete[] outer[i];
	}
	delete[] outer;
}

void C_switcher::execute(unsigned long dtime){
	for (int i = 0; i < n_inner; i++){
		for (int j = 0; j < inner_key_count; j++){
			inner[i][j]->execute(dtime);
		}
	}
	for (int i = 0; i < n_outer; i++){
		for (int j = 0; j < outer_key_count; j++){
			outer[i][j]->execute(dtime);
		}
	}
}

C_player & C_switcher::player(int line, inner_key key){
	return *inner[line][(int) key];
}

C_player & C_switcher::player(int line, outer_key key){
	line -= n_inner;
	return *outer[line][(int) key];
}
