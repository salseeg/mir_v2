#ifndef __MIR__MASK_ACTIONS
#define __MIR__MASK_ACTIONS

enum mask_action{
	mask_action__connect = 0,	//	���������
	mask_action__connect_local,	//	��������� � ����������
	mask_action__redial,		//	������� ��������� �����
	mask_action__redirect,		//	�������� ����������
	mask_action__reserve,		//	�������������� ������� �����
	mask_action__direct_connect,	//	������ ����������� �� ���� ����� ��� ����
	mask_action__block_incoming,	//	���������� �������� ����������
	mask_action__block_notify	//	���������� �����������
};
const int mask_action_count = 8;

extern const char * mask_action_names[];

#endif
