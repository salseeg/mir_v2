#ifndef __MIR__OUTER_PLAYER
#define __MIR__OUTER_PLAYER
/**********************************************
*
*	Реализация плейера 
*	для внешней линии
* 
**********************************/
#include "player.h"
#include "hard.h"
#include "aux/ring_.h"

class C_outer_player:public C_player{
	private:
		int line_id;
		outer_key key;
		unsigned long time;
		C_ring_<C_melody> q;

		void finish();
	public:
		C_outer_player(int line, outer_key k);
		~C_outer_player();
		
		void add(melody_name name);
		void reset();
		void execute(unsigned long dtime);
		int q_length();
};

#endif
