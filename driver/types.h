#ifndef MIR__TYPES_H
#define MIR__TYPES_H

#include "constants.h"

/*	Сосотояние линии	*/
struct status_bit{
	short state_fixed;				/*	фиксированное состояние (SH_CONNECTED/SH_BROKEN)	*/
	short state_flow;				/* 	текущее	сотояние (SH_CONNECTED/SH_BROKEN)	*/
	unsigned long time_fixed;			/* 	длительность фиксированного состояния (мкс)	*/
	unsigned long time_flow;			/*	длительность текущего состояния	(мкс)	*/
	unsigned short powered;
};

struct digit_recognition{
	short	count;				/*	счетчик количества импульсов	*/
	short	may_count;			/*	флаг позволяющий (при 1) посчитать импульс	*/
};

/*	Таймер для подачи разнообразных сигналов : занято, вызов, контроль вызова, попытка дозвона	*/
struct timer{
	short	type;				/*	тип сигнала	*/
	short	state;				/*	состояние (0/1)	*/
	long	time_to_change;			/*	время до смены состояния	*/
	short	owner;				/*	инициатор сигнала (матричный номер)	*/
	int	count;
};

struct service_buffer{
	short length;				/*	длина серии цифр	*/
	short digits[NUMBER_LENGTH];		/*	набираемая последоывательность	*/
};

/**********************************************************************************************************/
struct status_array{
	struct status_bit inner[N_INNER], outer[N_OUTER];
	long	delta;					/*	изменение времени с предыдущего вызова	*/
};

struct digits_array{
	struct digit_recognition inner[N_INNER],outer[N_OUTER];
};

struct timers_array{
	struct timer inner[N_INNER],outer[N_OUTER];
};

struct states_array{
	short	inner[N_INNER],outer[N_OUTER];		/* 	стадия жизненного цикла	*/
};

struct last_numbers_array{
	short 	inner[N_INNER][NUMBER_LENGTH];		/*	последний набранный номер	*/
};

struct line_numbers_array{
	short	inner[N_INNER],outer[N_OUTER];		/*	номер для каждой линии	*/
};
 
struct connection{
	short 	lines[N_LINES];				/*	кто из абонентов проключен в соединении	*/
	short	quantity;				/*	кол-во абонентов в соединении	*/
};

struct cur_con{
	short  inner[N_INNER],outer[N_OUTER];		/*	текущие соединения	*/
};

struct main_lines_array{
	short inner[N_INNER],outer[N_OUTER];		/*	номер линии главного абононента	*/
};

struct srv_buf_array{
	struct service_buffer inner[N_INNER];
};
/***********************************************************************************************************/	
struct station{
	struct states_array states;			/*	стадии жизненого цикла	*/
	struct status_array status;			/*	состояние каждой линии	*/
	struct timers_array timers;			/*	таймер	*/
	struct digits_array digits;			/*	набираемая цифра	*/
	struct last_numbers_array last_numbers;		/*	последний набранный номер	*/
	struct line_numbers_array line_numbers;		/*	номера для линий	*/
	unsigned char registers[N_REGS];		/*	выходные регистры	*/
	short next_free_connection;			/*	индекс следующего свободного соед (-1 ?)	*/
	struct connection connections[N_CONN];		/*	массив соединений == матрица	*/
	short	inner2matrix[N_INNER];			/*	номер внутр линии - номер на матрице	*/
	short	outer2matrix[N_OUTER];			/*	номер внешн линни - номер на матрице	*/
	short	special2matrix[N_SPECIAL];		/*	номер спец линии  - номер на матрице	*/ /*	музыка	*/	
	short	incoming_connections[N_INNER];		/*	номер входящего соединения	*/
	struct	cur_con current_connections;		/*	номер текущего соединения	*/	
	struct 	main_lines_array main_lines;		/*	номера линий главных абонентов	*/
	struct 	srv_buf_array service_buffers;		/*	последовательности набираемых цифр в режиме дополнительных услуг	*/
	short 	line_owners[N_OUTER];			/* 	line reserving	*/
};

#endif
