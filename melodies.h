#ifndef __MIR__MELODIES
#define __MIR__MELODIES

#include "melody.h"
#include "aux/array_.h"

enum melody_name{		// Мелодии	
	melody__digit_0 = 0,	//	импульсный 0
	melody__digit_1,	//	импульсная 1
	melody__digit_2,	//	импульсная 2
	melody__digit_3,	//	импульсная 3
	melody__digit_4,	//	импульсная 4
	melody__digit_5,	//	импульсная 5
	melody__digit_6,	//	импульсная 6
	melody__digit_7,	//	импульсная 7
	melody__digit_8,	//	импульсная 8
	melody__digit_9,	//	импульсная 9
	melody__ring,		//	посылка вызова (ПВ)
	melody__ring_control,	// 	контроль посылки вызова (КПВ)
	melody__busy,		//	занято
	melody__deny,		//	отказано в обслуживании
	melody__notify,		//	уведомление
	melody__ready,		//	готовность станции
	melody__ok,		//	услуга выполнена
	melody__got_connected,	//	включение сервисов соединения
	melody__initial_pause	//	занятие внешнего шлейфа
};

class C_melodies{
	private:
		C_array_<C_melody> array;
	public:
		C_melodies(int count);
		~C_melodies();
		C_melody *& operator[](melody_name n);
};

extern C_melodies * Melodies;

#endif
