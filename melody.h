#ifndef __MIR__MELODY
#define __MIR__MELODY

#include "aux/ring_.h"


enum melody_state{			// ��������� �������
	melody_state__error = -1,	//	������
	melody_state__low,		//	�� �������������� ������
	melody_state__high		//	�������������� ������
};

struct melody_frame{			// ���� �������
	unsigned long duration;		//	������������ �����
	enum melody_state state;	//	��������� ������� � ������� �����		
};

class C_melody{
	private:
		bool loop;			//	����������� ������� ?
		long unsigned duration;		//	������������ ������ ����� �������
		C_ring_<melody_frame> frames;	//	����� �������
	public:
		C_melody(bool looped);
		~C_melody();
		
		void add_frame(melody_frame frame);		//	�������� ����
		melody_state get_state(unsigned long time);	//	�������� ��������� � ��������� �� ������ �������
		bool get_looped();				//	�������� �� ������� ?
};
#endif
