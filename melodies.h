#ifndef __MIR__MELODIES
#define __MIR__MELODIES

#include "melody.h"
#include "aux/array_.h"

enum melody_name{		// �������	
	melody__digit_0 = 0,	//	���������� 0
	melody__digit_1,	//	���������� 1
	melody__digit_2,	//	���������� 2
	melody__digit_3,	//	���������� 3
	melody__digit_4,	//	���������� 4
	melody__digit_5,	//	���������� 5
	melody__digit_6,	//	���������� 6
	melody__digit_7,	//	���������� 7
	melody__digit_8,	//	���������� 8
	melody__digit_9,	//	���������� 9
	melody__ring,		//	������� ������ (��)
	melody__ring_control,	// 	�������� ������� ������ (���)
	melody__busy,		//	������
	melody__deny,		//	�������� � ������������
	melody__notify,		//	�����������
	melody__ready,		//	���������� �������
	melody__ok,		//	������ ���������
	melody__got_connected,	//	��������� �������� ����������
	melody__initial_pause	//	������� �������� ������
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
