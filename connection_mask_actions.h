
#ifndef __MIR__CONNECTION_MASK_ACTIONS
#define __MIR__CONNECTION_MASK_ACTIONS

enum connection_mask_action{
	connection_mask_action__hold = 0,	//	��������
	connection_mask_action__mix,		//	��������� � ��������� 
	connection_mask_action__mix_without,	//	���������� � ��������� ���� ����� ����������
	connection_mask_action__auto_hold	//	���� ��������� = ��������� + ����������� �� ����������
};
const connection_mask_action_count = 4;

extern const char * connection_mask_action_names[];

#endif
