#ifndef __MIR__LINE
#define __MIR__LINE

//
//	Линия - базовый для входных и выходных линий
//	

#include "aux/ring_.h"

class C_line;

#include "connection.h"
#include "station.h"
#include "pulse_recognizer.h"

enum line_signal{			// Сигналы линии
	line_signal__line_deleted,	//	отключилась линия от тукущего соединения
	line_signal__line_added,	//	добавилсь линия в соединение
	line_signal__line_dialed,	//	внешняя линия закончила набор номера
	line_signal__holded_by,		//	текущее соед линии удержано
	line_signal__unholded_by,	//	инициатор удержания вернулся в соединение
	line_signal__holded_freed	//	удержанное соединение освобождено
};
const int line_signal_count = 6;

enum line_stage {			// Жизненый цикл(ЖЦ) линии
	line_stage__free,		//	свободен
	line_stage__ready,		//	готов - станция ожидает набора
	line_stage__digits,		//	набор - станция принимает цифры
	line_stage__wait,		//	ожидает соединения
	line_stage__retranslation,	//	ожидает завершения набора
	line_stage__connect,		//	в соединении
	line_stage__disconnected,	//	соединение завершено
	line_stage__incoming		//	ожидание соединения с оператором (внешн)
};
const int line_stage_count = 8;

enum line_type {		// Типы линий
	line_type__inner,	//	внутренняя
	line_type__outer	//	внешняя
};

class C_line{
	private:
		char * last_number;
	protected:
		int id;					//	номер линии (на матрице)
		
		typedef void (C_line::* stage_handler)();
		static stage_handler stage_handlers[line_stage_count];
		
		virtual void stage__free()=0;
		virtual void stage__ready()=0;
		virtual void stage__digits()=0;
		virtual void stage__wait()=0;
		virtual void stage__retranslation()=0;
		virtual void stage__connect()=0;
		virtual void stage__disconnect()=0;
		virtual void stage__incoming()=0;
		
		typedef void (C_line::* signal_handler)();
		static signal_handler signal_handlers[line_signal_count];

		virtual void signal__line_deleted()=0;
		virtual void signal__line_added()=0;
		virtual void signal__line_dialed()=0;
		virtual void signal__holded_by()=0;
		virtual void signal__unholded_by()=0;
		virtual void signal__holded_freed()=0;
		
	public:
		line_stage stage;			//	стадия ЖЦ
		C_connection * current_connection;	//	текущее соединение
		C_connection * incoming_connection;	//	
		int client_id;				//	профиль абонента
		int own_bus_id;				//	собственный пучек
		int operator_bus_id;			//	пучек оператора
		int owner_id;				//	ид линии зарезервировавшей эту (-1 = нет владельца)
		bool block_incoming;			//	блокировка входящих соединений
		bool block_notify;			//	блокировка уведомлений
		bool blocked;				//	блокировка для обработки одновременного обращения к линии
		int priority;				//	приоритет линии
		
		C_pulse_recognizer * recognizer;
		C_ring_<C_connection> hold_ring;	//	удержанные соединения
		
		C_line(int id);
		virtual ~C_line();
		
		int get_id();				//	узнать номер линии
		virtual int execute();			//	выполнить обработку линии

		virtual void remember_number();
		virtual void restore_number();
		char * get_last_number(){return last_number;}
		
		int operator==(const C_line& b){ return id == b.id;}
		int operator!=(const C_line& b){ return id != b.id;}

		virtual bool active()=0;		//	активная линия (управляет пасивными(внешними) в соединении)
					
		void signal(line_signal sig);
		virtual void switch_stage(line_stage new_stage);
};

#endif 
// 	
