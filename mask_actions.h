#ifndef __MIR__MASK_ACTIONS
#define __MIR__MASK_ACTIONS

enum mask_action{
	mask_action__connect = 0,	//	соединить
	mask_action__connect_local,	//	соединить с внутренним
	mask_action__redial,		//	набрать последний номер
	mask_action__redirect,		//	передать полномочия
	mask_action__reserve,		//	резервирование внешней линии
	mask_action__direct_connect,	//	прямое проключение на внеш линию без ДВОС
	mask_action__block_incoming,	//	блокировка входящих соединений
	mask_action__block_notify	//	блокировка уведомлений
};
const int mask_action_count = 8;

extern const char * mask_action_names[];

#endif
