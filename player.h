#ifndef __MIR__PLAYER
#define __MIR__PLAYER
/***************************************************
*
* 	Плейер. Интерфейс.
*
***********************************/
#include "melodies.h"

class C_player{
	public:
		virtual ~C_player();
		
		virtual void add(melody_name name)=0;
		virtual void reset() = 0;
		virtual void execute(unsigned long dtime)=0;
		virtual int q_length()=0;
};

#endif
