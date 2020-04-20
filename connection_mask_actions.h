
#ifndef __MIR__CONNECTION_MASK_ACTIONS
#define __MIR__CONNECTION_MASK_ACTIONS

enum connection_mask_action{
	connection_mask_action__hold = 0,	//	удержать
	connection_mask_action__mix,		//	обединить с удержаным 
	connection_mask_action__mix_without,	//	объединить с удержаным всех кроме инициатора
	connection_mask_action__auto_hold	//	авто удержание = удержание + проключение на локального
};
const int connection_mask_action_count = 4;

extern const char * connection_mask_action_names[];

#endif
