#ifndef __MIR__MASK
#define __MIR__MASK
//
//	Маска
//	телефонных номеров и
//	дополнительных услуг
//
#include "mask_actions.h"
#include "aux/ring_.h"

enum mask_check{
	mask_check__completed,		//	маска совпала
	mask_check__not_completed,	//	маска может совпасть необходимо больше цифр
	mask_check__not_match,		//	маска не подходит
	mask_check__not_allowed		//	нет прав для доступа к маске
};

class C_mask{
	private:		
		char * number;		//	маска номера
		C_ring_<int> secure;	//	профили доступа

		bool check_permission(int line_id);
	public:
		int skip_len;		//	длина префикса доступа
		mask_action func;	//	вид услуги
		int bus_id;		//	номер пучка

		C_mask();
		~C_mask();
		
		int set(char * numb, char * f, int b_id, int skip);
		void allow_client(int client_id);
		mask_check check(int line_id);
};

#endif
