
#include "connection_mask.h"
#include "station.h"
#include <stdlib.h>

C_connection_mask::C_connection_mask(){
	number = NULL;	
	func = connection_mask_action__hold;
	arg = -1;
}

C_connection_mask::~C_connection_mask(){
	if (number) delete[] number;
	int n = secure.quantity();
	for (int i = 0; i < n; i++){
		secure.pop();
	}
}

int C_connection_mask::set(char * numb, char * f, int arg_){
	arg = arg_;
	if (number) delete[] number;
	number = new char[::strlen(numb) + 1];
	::strcpy(number,numb);
	for (int i = 0; i < connection_mask_action_count; i++){
		if (!::strcmp(connection_mask_action_names[i], f)){
			func = (connection_mask_action) i;
			return 0;
		}
	}
	return -1;
}

void C_connection_mask::allow_client(int client_id){
	secure.add(new int(client_id));
}

bool C_connection_mask::check_permission(int line_id){
	int client_id = Station->lines[line_id]->client_id;
	int n = secure.quantity();
	int i;
	for (i = 0; i < n; i++, secure.roll(1)){
		if (*secure.get() == client_id){
			break;
		}			
	}
	return  (i != n);
}

connection_mask_check C_connection_mask::check(int line_id){
	int m_len = ::strlen(number);
	C_pulse_recognizer * pr = Station->lines[line_id]->recognizer;
	int r_len = pr->get_length();

	if (r_len > m_len) return connection_mask_check__not_match;
	connection_mask_check res = connection_mask_check__not_completed;
	for (int i = 0; i < m_len; i++){
		if (i == r_len){			
			res = connection_mask_check__not_completed;
			break;
		}else if (number[i] == ((*pr)[i] + 0x30)){
		}else if (number[i] == 'x'){
		}else{
			res = connection_mask_check__not_match;
			break;
		}
	}
	if ((r_len == m_len) && (res == connection_mask_check__not_completed)){
		res = check_permission(line_id) ? 
			connection_mask_check__completed :
			connection_mask_check__not_allowed;
	}
	return res;
}
