
#ifndef __MIR__CONNECTION_MASK
#define __MIR__CONNECTION_MASK
//
//	�����
//	���������� ������� �
//	�������������� �����
//	��������� � �����������
//
#include <mir/connection_mask_actions.h>
#include <mir/aux/ring_.h>

enum connection_mask_check{
	connection_mask_check__completed,	//	����� �������
	connection_mask_check__not_completed,	//	����� ����� �������� ���������� ������ ����
	connection_mask_check__not_match,	//	����� �� ��������
	connection_mask_check__not_allowed	//	��� ���� ��� ������� � �����
};

class C_connection_mask{
	private:		
		char * number;		//	����� ������
		C_ring_<int> secure;	//	������� �������

		bool check_permission(int line_id);
	public:
		connection_mask_action func;	//	��� ������
		int arg;			//	��� �������� (�������� ����� ��� ����� ��� �������������)

		C_connection_mask();
		~C_connection_mask();
		
		int set(char * numb, char * f, int arg_);
		void allow_client(int client_id);
		connection_mask_check check(int line_id);
};
#endif
