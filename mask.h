#ifndef __MIR__MASK
#define __MIR__MASK
//
//	�����
//	���������� ������� �
//	�������������� �����
//
#include "mask_actions.h"
#include "aux/ring_.h"

enum mask_check{
	mask_check__completed,		//	����� �������
	mask_check__not_completed,	//	����� ����� �������� ���������� ������ ����
	mask_check__not_match,		//	����� �� ��������
	mask_check__not_allowed		//	��� ���� ��� ������� � �����
};

class C_mask{
	private:		
		char * number;		//	����� ������
		C_ring_<int> secure;	//	������� �������

		bool check_permission(int line_id);
	public:
		int skip_len;		//	����� �������� �������
		mask_action func;	//	��� ������
		int bus_id;		//	����� �����

		C_mask();
		~C_mask();
		
		int set(char * numb, char * f, int b_id, int skip);
		void allow_client(int client_id);
		mask_check check(int line_id);
};

#endif
