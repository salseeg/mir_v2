
#ifndef __MIR__CONNECTION_MASK
#define __MIR__CONNECTION_MASK
//
//	Маска
//	телефонных номеров и
//	дополнительных услуг
//	доступных в соединениии
//
#include "connection_mask_actions.h"
#include "aux/ring_.h"

enum connection_mask_check{
	connection_mask_check__completed,	//	маска совпала
	connection_mask_check__not_completed,	//	маска может совпасть необходимо больше цифр
	connection_mask_check__not_match,	//	маска не подходит
	connection_mask_check__not_allowed	//	нет прав для доступа к маске
};

class C_connection_mask{
	private:		
		char * number;		//	маска номера
		C_ring_<int> secure;	//	профили доступа

		bool check_permission(int line_id);
	public:
		connection_mask_action func;	//	вид услуги
		int arg;			//	доп агрумент (например номер лок линии для автоудержания)

		C_connection_mask();
		~C_connection_mask();
		
		int set(char * numb, char * f, int arg_);
		void allow_client(int client_id);
		connection_mask_check check(int line_id);
};
#endif
