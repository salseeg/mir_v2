#ifndef __MIR__PULSE_RECOGNIZER
#define __MIR__PULSE_RECOGNIZER
/***************************************************
*
*	������������� ����������� ������
*
****************************************/ 
#include "aux/array_.h"
#include "interface/driver.h"
#include <stdlib.h>

enum recognition_result{			// ��������� �������������
	recognition_result__digit,		//	����� ���������
	recognition_result__not_started,	//	����� �� ������
	recognition_result__not_ended,		//	����� �� ���������
	recognition_result__error		//	������
};

class C_pulse_recognizer{
	private:
		char count;			//	���-�� ���������
		char may_count;			//	����� �� ������� ��������
		char * array;			//
		int length;			//
	public:
		C_pulse_recognizer(char * s = NULL);
		~C_pulse_recognizer();

		char operator[](int index);		// [0-9]*
		int get_length(){return length;}
		int retranslated_count;			// ������� ��������� ���� // ���������������� �����
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
