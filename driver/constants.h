/***************************************************************
 * 
 * 
 *
 *	Константы.
 *	
 *	TODO Через конфиг файл
 *
 *
 */ 

#ifndef MIR__CONSTANTS_H
#define MIR__CONSTANTS_H

#define N_INNER 		8 		  /*	кол-во внутр линий	*/
#define N_OUTER 		4		/* 	кол-во внеш линий	*/
#define N_LINES			16		/*	кол-во линий	*/
#define N_CONN			8		/*	кол-во соединений	*/
#define N_SPECIAL		1		/*	кол-во спец линий	*/
#define NUMBER_LENGTH		20		/*	кол-во цифр в номере	*/

#define BASE_PORT		0x150		/*	базовый порт	*/
#define N_REGS			7		/*	кол-во регистров	*/

/*	Смещения для регистров	*/
#define OFS_PV			0		/*	посылка вызова	*/
#define	OFS_KNA			1		/*	ключи зумера АК	*/
#define OFS_MT_ADR		2		/*	адрес на матрице	*/
#define OFS_MT_SS		3		/*	строб и селект на матрицу	*/
#define OFS_R_SH		4		/*	реле шлейфа	*/
#define OFS_IK			5		/*	импульсный ключ	*/
#define OFS_SK			6		/*	шунтирующий ключ	*/

/*		*/
#define NO_CONNECTION		-1		/*	текущее и ожидающее соединение	*/

#define	NO_OWNER		-1		/*	инициатор таймера	*/

#define M_ON			1		/*	включение комутации	*/
#define M_OFF			0		/*	выключение комутации	*/

#define T_ST_HI			1		/*	импульс сигнала таймера	*/
#define T_ST_LO			0		/*	пауза сигнала таймера	*/

/*	Состояния шлейфа	*/
#define SH_BROKEN		0		/*	шлейф разомкнут */
#define	SH_CONNECTED		1		/*	шлейф замкнут	*/

/*	Жизненый цикл внешних линий	*/
#define ST_O_FREE		0
#define ST_O_INNER_CONNECTION	5
#define ST_O_CALL		2
#define ST_O_CONNECTION		3
#define ST_O_WAIT_4_CONNECT	4		/*	подача вызова, внешний ждет	*/	
#define ST_O_DISCONNECTED	1		/*	подавление ложного вызова	*/
#define ST_O_REDIAL_DIGIT	6		/*	набор последнего набранного номера	*/
#define ST_O_REDIAL_PAUSE	7		/* 	pause between digits */

/*	Жизненый цикл внутренних линий	*/
#define	ST_I_DISCONNECTED	0		/*	передача "занято"	*/
#define ST_I_FREE		1		/* 	трубка положена		*/	
#define ST_I_READY		2		/*	трубка поднята		*/
#define ST_I_FIRST_DIGIT	3		/*	набор перой цифры	*/
#define ST_I_WAIT_4_CONNECT	4		/*	вызов абонента		*/
#define ST_I_CONNECTION		5		/*	соединение		*/
#define ST_I_CH_0_LINE		6		/*	выбор внешн линии	*/
#define ST_I_NUM_RETRANSLATION	7	/*	ретрансляция номера	*/
#define ST_I_SERVICE		8		/*	ДВО			*/
#define ST_I_CONNECTION_SERVICE	9	/*	ДВО в разговоре	*/
#define ST_I_SECOND_DIGIT	10		/*	внутр абоненты	*/
#define ST_I_PRE_CONNECTION	11		/*	дождаться окончания набора	*/

#define OUTER_REQ_DISCONNECT_TIMEOUT	900000		/*	подавление ложного вызова	*/
#define READY_TIMEOUT			20000000	/*	таймаут ожидания первой цифры	*/
#define	WAIT_4_CONNECT_TIMEOUT		45000000	/*	таимаут ожидания соединения	*/
#define OUTER_INC_RING_TIMEOUT		4000000		/*	пауза вызывающего городского сигнала	*/
#define REQUIRED_DISCONNECT_TIME	200000		/*	минимум времени до перехода в свобоное состояние	*/ 
#define TIME_2_MAIN			2500000		/*	время до проключения на главного при снятии трубки	*/

#define RETRANSLATION_NUMBER	5		/*	необходимое кол-во цифр для включения сервисов(переход в ST_I_CONNECTION)	*/
#define END_OF_NUMBER   	60000000	/*	таймаут окончания номера	*/


#define DIGIT_OUTER		0		/*	выход на внешние	*/
//#define DIGIT_SERVICE		1		/*	выход на сервис		*/

/*	Сигналы		*/
#define	SIG_NO			-1		/*	нет сигнала		*/
#define SIG_RING		0		/*	вызов			*/
#define SIG_BUSY		1		/*	занято			*/
#define SIG_DENY		2		/*	перегрузка		*/
#define SIG_NOTIFY		3		/*	уведомление		*/
#define SIG_CTRL		4		/*	контроль вызова		*/
#define SIG_READY		5		/*	ответ станции "готова"	*/
#define	SIG_OK			6		/*	услуга выполнена успешно	*/
#define SIG_REDIAL		7		/*	набор последнего набранного номера	*/
#define SIG_REDIAL_INTER	8		/*	межсерийная пауза	*/
#define SIG_CONNECTION		9		/*	при переходе из ретрансляции номера в соединение	*/

#define SI_COUNT_INFINITY	-1

#define SIG_RING_PULSE		800000
#define SIG_RING_PAUSE		3200000
#define SIG_RING_COUNT		SI_COUNT_INFINITY
#define SIG_BUSY_PULSE		300000
#define SIG_BUSY_PAUSE		300000
#define SIG_BUSY_COUNT		SI_COUNT_INFINITY
#define SIG_DENY_PULSE  	50000
#define SIG_DENY_PAUSE  	50000
#define	SIG_DENY_COUNT		SI_COUNT_INFINITY
#define SIG_NOTIFY_PULSE	250000
#define SIG_NOTIFY_PAUSE        2525000
#define SIG_NOTIFY_COUNT	SI_COUNT_INFINITY
#define SIG_CTRL_PULSE		800000
#define SIG_CTRL_PAUSE		3200000
#define SIG_CTRL_COUNT		SI_COUNT_INFINITY
#define	SIG_READY_PULSE		40000000
#define	SIG_READY_PAUSE		0
#define SIG_READY_COUNT		SI_COUNT_INFINITY
#define	SIG_OK_PULSE		100000
#define SIG_OK_PAUSE    	300000
#define SIG_OK_COUNT		SI_COUNT_INFINITY
#define SIG_REDIAL_PULSE	60000

#define SIG_REDIAL_PAUSE	40000
#define SIG_REDIAL_COUNT	SI_COUNT_INFINITY

#define SIG_REDIAL_INTER_PULSE	600000
#define SIG_REDIAL_INTER_PAUSE	0
#define SIG_REDIAL_INTER_COUNT	1
#define SIG_CONNECTION_PULSE	100000
#define SIG_CONNECTION_PAUSE	300000
#define SIG_CONNECTION_COUNT	3



/*	Константы для определения номера (мкс)	*/
#define	PULSE_TIME_MIN		20000		/*	минимальное время импульса	*/
#define PULSE_TIME_MAX		200000		/*	максимальное время импульса	*/
#define PAUSE_TIME_MIN		20000		/*	минимальное время паузы		*/
#define PAUSE_TIME_MAX		200000		/*	максимальное время паузы	*/
#define INTER_TIME_MIN		250000		/*	минимальное время межсерийной паузы	*/
#define INTER_TIME_MAX		600000000	/*	максимальное время межсерийной паузы	*/

/*	Ошибки recognize_digit()	*/
#define	E_DIGIT_RECOGNITION_ERROR	-1	/*	сигнал имеет неправильную форму (слишком длинный) =>"занято"	*/
#define E_DIGIT_RECOGNITION_NOT_ENDED	-2	/*	конец серии не наступил, считаю импульсы	*/
#define	E_DIGIT_RECOGNITION_NOT_STARTED	-3	/*	серия не началась, жду	*/

/* 	read_phone_masks()	*/
#define READ_PHONE_MASKS_BUF_LEN 	240

/*	check_phone_masks()	Проверка соответствия номера маске	*/
#define E_MASK_NOT_FOUND		0	/*	Соотв маска не найдена	*/
#define E_MASK_NOT_COMPLITED		1	/*	Есть маски начинающиеся так же	*/
#define E_MASK_COMPLITED		2	/*	Номер соотв маске	*/


#endif

