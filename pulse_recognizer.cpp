#include "pulse_recognizer.h"
#include "log/log.h"
#include <stdlib.h>
#include <string.h>

// Характеристики входного набора (мкс)
unsigned long recognition__low_time_min   =    20000;	// минимальное время паузы
unsigned long recognition__low_time_max   =   200000;	// максимальное время паузы
unsigned long recognition__high_time_min  =    20000;	// минимальное время импульса
unsigned long recognition__high_time_max  =   200000;	// максимальное время импульса
unsigned long recognition__inter_time_min =   250000;	// минимальное время между цифрами
unsigned long recognition__inter_time_max = 60000000;	// максимальное время между цифрами
// inter_time_max мешает постоянному распознанию номера в соединении

C_pulse_recognizer::C_pulse_recognizer(char * s = NULL):
	count(0), 
	may_count(0), 
	array(NULL), 
	length(0),
	retranslated_count(0)
{	
	if (s){
		int n = ::strlen(s);	
		length = n;
		array = new char[n];
		for (int i = 0; i < n; i++){
			array[i] = s[i] - '0';
		}
	}
}

recognition_result C_pulse_recognizer::recognize(bit_history& bit){
	if (bit.flow_time == 0) return recognition_result__not_started;
	if (bit.fixed_time == 0) return recognition_result__not_started;
	if (bit.flow_state == bit_state__off){
		// положительный переход	
		if ((bit.flow_time > recognition__high_time_max) ||
		   ((bit.fixed_time >= recognition__low_time_max) && 
		    (bit.fixed_time <= recognition__inter_time_min))){
			count = 0;
			return recognition_result__error;
		}else{
			may_count = 1;
		}
	}else{
		// отрицательный переход
		//if (bit.flow_time > recognition__inter_time_max){
		//	count = 0;
		//	return recognition_result__error;
		//}
		if (bit.flow_time > recognition__inter_time_min){
			if (count > 0){
				char * p = array;
				array = new char[length + 1];
				for (int i = 0; i < length; i++)
					array[i] = p[i];
				if (p) delete p;
				array[length++] = (count % 10);				
				//Log->set_priority(log_priority__info);
				//Log->rec() << count %10 ;
				//Log->write();
				count = 0;
				return recognition_result__digit;
			}
			return recognition_result__not_started;
		}
		if (may_count && bit.fixed_time < recognition__high_time_max){
			count++;
			may_count = 0;
		}
	}
	return recognition_result__not_ended;
}

char C_pulse_recognizer::operator[](int index){
	if (index < 0) return -1;
	if (index >= length) return -1;
	return array[index];
}

C_pulse_recognizer::~C_pulse_recognizer(){
	if (array) delete array;
}

C_pulse_recognizer::operator char *(){
	char * s = new char[length + 1];
	for (int i = 0; i < length; i++){
		s[i] = array[i] + '0';
	}
	s[length] = '\0';
	return s;
}
