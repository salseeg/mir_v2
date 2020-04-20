#ifndef __MIR__MELODY
#define __MIR__MELODY

#include "aux/ring_.h"


enum melody_state{			// Состояние мелодии
	melody_state__error = -1,	//	ошибка
	melody_state__low,		//	не воспроизводить сигнал
	melody_state__high		//	воспроизводить сигнал
};

struct melody_frame{			// Кадр мелодии
	unsigned long duration;		//	длительность кадра
	enum melody_state state;	//	состояние мелодии в течение кадра		
};

class C_melody{
	private:
		bool loop;			//	циклическая мелодия ?
		long unsigned duration;		//	длительность одного цикла мелодии
		C_ring_<melody_frame> frames;	//	кадры мелодии
	public:
		C_melody(bool looped);
		~C_melody();
		
		void add_frame(melody_frame frame);		//	добавить кадр
		melody_state get_state(unsigned long time);	//	получить состояние в указанный от начала моммент
		bool get_looped();				//	циклична ли мелодия ?
};
#endif
