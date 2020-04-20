#ifndef __MIR__DRIVER_INTERFACE
#define __MIR__DRIVER_INTERFACE

//
//	Структуры взаимодействия с драйвером
//	

/*******************************************
*
*	Структуры для чтения из драйвера
* 
*****************************/ 
enum bit_state {			// Состояние бита	
	bit_state__off = 0,		//	низкое	(0)
	bit_state__on			//	высокое	(1)
};

enum power_state{			// Питание линии
	power_error = 0,		//	отсутствует
	power_ok = 1			//	в порядке
};

struct bit_history{			// История бита
	unsigned long fixed_time;	//	время в предыдущем состоянии (мкс)
	unsigned long flow_time;	//	время в текущем состоянии (мкс)
	enum bit_state flow_state;	//	текущее состояние (обратное предыдущему)
	enum power_state powered;	//	есть ли питание на линии (для внешних)
};

enum LT{				// Тип линии
	LT_inner = 0,			//	внутренняя линия
	LT_outer			//	внешняя линия
};

struct read_line_states{		// Чтение состояний линий 
	unsigned long delta_time;	//	изменеие времени относительно предыдущего вызова
	struct bit_history * bits;	//	массив бит ,длиной = quantity
	enum LT line_type;		//	тип линий
	unsigned short quantity;	//	кол-во линий в массиве
};

/****************************************
*
*	Структуры для записи в драйвер
* 
******************/
enum command_type{ 	// Тип команды
	CT_matrix,	//	проключение матрицы 		(шнурок, линия, вкл/выкл)
	CT_KNA,		//	зумер внутр линии 		( , внутр линия, вкл/выкл)
	CT_PV,		//	посылка вызова(звонок) внутр линии ( , внутр линия, вкл/выкл)
	CT_KV,		//	разг тракт внеш линии		( , внеш линия, вкл/выкл)
	CT_IK,		//	импульсный ключ внеш линии 	( , внеш линия, вкл/выкл)
	CT_SK,		//	шунтирующий ключ внеш линии 	( , внеш линия, вкл/выкл)
	CT_sleep,	//	обработка завершена
	CT_init		//	инициализация аппаратной части
};

enum on_off{		// Вкл/выкл
	off = 0,	//	выкл
	on = 1		//	вкл
};		

struct write_command{		// Команда записи
	enum command_type type;	//	тип команды
	char cord_number;	//	номер шнурка матрицы
	char line_number;	//	номер линии {внеш/внутр/на матрице}
	enum on_off state;	//	необходимое состояние
};

#endif
