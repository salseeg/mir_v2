#ifndef __MIR__PULSE_RECOGNIZER
#define __MIR__PULSE_RECOGNIZER
/***************************************************
*
*	Распознование импульсного набора
*
****************************************/ 
#include "aux/array_.h"
#include "interface/driver.h"
#include <stdlib.h>

enum recognition_result{			// Результат распознования
	recognition_result__digit,		//	цифра закончена
	recognition_result__not_started,	//	цифра не начата
	recognition_result__not_ended,		//	цифра не закончена
	recognition_result__error		//	ошибка
};

class C_pulse_recognizer{
	private:
		char count;			//	кол-во импульсов
		char may_count;			//	можно ли считать импульсы
		char * array;			//
		int length;			//
	public:
		C_pulse_recognizer(char * s = NULL);
		~C_pulse_recognizer();

		char operator[](int index);		// [0-9]*
		int get_length(){return length;}
		int retranslated_count;			// счетчик набранных цифр // небуферизируемый набор
		operator char *();
		recognition_result recognize(bit_history& bit);		
};

extern unsigned long recognition__low_time_min;
extern unsigned long recognition__low_time_max;
extern unsigned long recognition__high_time_min;
extern unsigned long recognition__high_time_max;
extern unsigned long recognition__inter_time_min;
extern unsigned long recognition__inter_time_max;

#endif
