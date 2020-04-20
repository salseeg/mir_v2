#ifndef __MIR__LOG
#define __MIR__LOG
/*****************************************************************************************
*	
*	Журналирование сообщений
*	
*	Инициализация			Log = new C_log("application name");
*	Изменение приоритета		Log->set_priority(log_priority__alert);
*	Составление сообщения		Log->rec() << "Hello world" <<  34 << 35.6 << "\n";
*	Отправка сообщения в журнал 	Log->write();
*	Завершение работы с журналом	delete Log;
*
************************************/
#include <strstream.h>

//
//	Журнал.
//	используя syslog.
//

#define LOG_BUF_LENGTH 512


enum log_priority{
	log_priority__emergency = 0,
	log_priority__alert,
	log_priority__critical,
	log_priority__error,
	log_priority__warning,
	log_priority__notice,
	log_priority__info,
	log_priority__debug	
};


class C_log{
	private:
		enum log_priority priority;
		char buf[LOG_BUF_LENGTH];
		ostrstream * record;
	public:
		C_log(char * appname);
		~C_log();
		ostrstream& rec();
		void write();
		void set_priority(enum log_priority pr);
};

extern C_log * Log;

#endif
