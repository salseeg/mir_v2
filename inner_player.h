#ifndef __MIR__INNER_PLAYER
#define __MIR__INNER_PLAYER
/**********************************************
*
*	Реализация плейера 
*	для внутренней линии
* 
**********************************/
#include "player.h"
#include "hard.h"
#include "aux/ring_.h"

class C_inner_player:public C_player{
	private:
		int line_id;
		inner_key key;
		unsigned long time;
		C_ring_<C_melody> q;

		void finish();
	public:
		C_inner_player(int line, inner_key k);
		~C_inner_player();
		
		void add(melody_name name);
		void reset();
		void execute(unsigned long dtime);
		int q_length();
};

#endif
