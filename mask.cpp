#include <mir/mask.h>
#include <mir/station.h>
#include <stdlib.h>

C_mask::C_mask(){
	number = NULL;	
	skip_len = -1;
	func = mask_action__connect;
	bus_id = -1;
}

C_mask::~C_mask(){
	if (number) delete[] number;
	int n = secure.quantity();
	for (int i = 0; i < n; i++){
		secure.pop();
	}
}

int C_mask::set(char * numb, char * f, int b_id, int skip){
	bus_id = b_id;
	skip_len = skip;
	if (number) delete[] number;
	number = new char[::strlen(numb) + 1];
	::strcpy(number,numb);
	for (int i = 0; i < mask_action_count; i++){
		if (!::strcmp(mask_action_names[i], f)){
			func = (mask_action) i;
			return 0;
		}
	}
	return -1;
}

void C_mask::allow_client(int client_id){
	secure.add(new int(client_id));
}

bool C_mask::check_permission(int line_id){
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

mask_check C_mask::check(int line_id){
	int m_len = ::strlen(number);
	C_pulse_recognizer * pr = Station->lines[line_id]->recognizer;
	int r_len = pr->get_length();

	if (r_len > m_len) return mask_check__not_match;
	mask_check res = mask_check__not_completed;
	for (int i = 0; i < m_len; i++){
		if (i == r_len){			
			res = mask_check__not_completed;
			break;
		}else if (number[i] == ((*pr)[i] + 0x30)){
		}else if (number[i] == 'x'){
		}else{
			res = mask_check__not_match;
			break;
		}
	}
	if ((r_len == m_len) && (res == mask_check__not_completed)){
		res = check_permission(line_id) ? 
			mask_check__completed :
			mask_check__not_allowed;
	}
	return res;
}
