#ifndef __MIR__MASKS
#define __MIR__MASKS
//
//	Массив масок
//
#include "mask.h"
#include "aux/ring_.h"

class C_masks{
	private:
		typedef void (C_masks::* action)(int line_id, int skip_len,int bus_id);
		static action actions[mask_action_count];
		
		void action__connect(int line_id, int skip_len, int bus_id);
		void action__connect_local(int line_id, int skip_len, int bus_id);
		void action__redial(int line_id, int skip_len, int bus_id);
		void action__redirect(int line_id, int skip_len, int bus_id);
		void action__reserve(int line_id, int skip_len, int bus_id);
		void action__direct_connect(int line_id, int skip_len, int bus_id);
		void action__block_incoming(int line_id, int skip_len, int bus_id);
		void action__block_notify(int line_id, int skip_len, int bus_id);

		bool retranslate_recognized(int line_id, int bus_id, int skip_len);
	public:
		C_ring_<C_mask> ring;		//	кольцо масок
		void check(int line_id);	//	проверка маски и перевод линии в соотв сост
};

extern C_masks * Masks;

#endif
