
#ifndef __MIR__CONNECTION_MASKS
#define __MIR__CONNECTION_MASKS
//
//	������ �����
//	��������� � ����������
//
#include "connection_mask.h"
#include "aux/ring_.h"

class C_connection_masks{
	private:
		typedef void (C_connection_masks::* action)(int line_id, int arg);
		static action actions[connection_mask_action_count];
		
		void action__hold(int line_id, int);
		void action__mix(int line_id, int);
		void action__mix_without(int line_id, int);
		void action__auto_hold(int line_id, int arg);

	public:
		C_ring_<C_connection_mask> ring;	//	������ �����
		void check(int line_id);		//	�������� ����� � ������� ����� � ����� ����
};

extern C_connection_masks * Connection_masks;
#endif
