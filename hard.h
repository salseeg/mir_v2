#ifndef __MIR__HARD
#define __MIR__HARD
//
//	�������������� � ���������� �������
//
	
// TODO ����������� ��������� ������

#include "interface/driver.h"

enum inner_key{		// ����� ���������� �����
	inner_key__kna,	//	�������
	inner_key__pv	//	������� ������
};
const int inner_key_count = 2;

enum outer_key{		// ����� ������� �����
	outer_key__kv,	//	������
	outer_key__sk,	//	�����������
	outer_key__ik	//	����������
};
const int outer_key_count = 3;

class C_hard{
	private:
		int driver_descriptor;		//	���������� ��������
		int n_inner;			//	���-�� ����� �����
		int n_outer;			//	���-�� ����� �����
		
		bit_history * inner_states;	//	��������� ����� �����
		bit_history * outer_states;	//	��������� ����� �����
		
		on_off * kna;
		on_off * pv;
		on_off * kv;
		on_off * sk;
		on_off * ik;
		
		bool kna_m;
		bool pv_m;
		bool kv_m;
		bool sk_m;
		bool ik_m;

		unsigned long delta;
		
		void read();			//	������� ���������

	public:
		
		C_hard(char * driver_filename, int n_inner, int n_outer);
		~C_hard();

		void matrix(int cord, int matrix_line, on_off st);
		void sleep_us();
		void set_inner(int inner_line, inner_key key, on_off st);
		void set_outer(int outer_line, outer_key key, on_off st);
		bit_history & get_state_inner(int inner_line);
		bit_history & get_state_outer(int outer_line);

		
		unsigned long get_delta();
};

extern C_hard * Hard;

#endif
