#ifndef __MIR__SERVICE
#define __MIR__SERVICE
/***************************************************************
*	
*	Формат сообщений взаимодействия между клиентом
*	и станцией(плагином) описан структурой service_command
*		unsigned short id       - идентификатор команды 
*		long data_size		- размер данных
*		char data[data_size]	- данные
*	
*
*	Предопределенные идентификаторы команд
*	серверу:
*		0x0000	-  подключение
*			должна быть первой командой, отправляемой 
*			серверу, иначе разрыв соединения
*	клиенту:
*		0xfffe  -  сообщение об ошибке
*		0xffff  -  разорвать соединение
*
*	Каждый плагин должен содержать точку входа 
*		int execute(C_service * service);
*	возвращать -1 для разрыва соединения.
*		   -2  - выключения станции.
*
****************************************/

#include "socket.h"
#include <stdlib.h>

extern char * plugins_path;	//	путь к плагинам


class C_service_command{	//  Команда сервиса:
	private:
		char * data;		//	данные	
	public:
		unsigned short id;	//	идентификатор команды
		long data_size;		//	размер данных
			
		void set_data(const char *, long); 
		const char * get_data() const;
		
		C_service_command();
		C_service_command(unsigned short id_, long ds_, char * data_);
		C_service_command(const C_service_command & b);
		C_service_command& operator=(const C_service_command &b);
		~C_service_command();
};


class C_service{
	private:
		char * buf;			//	буфер
		int buf_len;			//	длина буфера
		int (* plugin)(C_service *);	//	обработчик сервиса
		void * plugin_handle;		//	хендл плагина
	public:
		C_socket * sock;		//	сокет
		char * service_name;		//	название сервиса
		
		C_service(C_socket * s);
		~C_service();
		C_service_command * read();		//	возвращает считаную команду, NULL - иначе
		void write(const C_service_command & command);	//	записывает команду
		int execute();				//	обработка
		void buf_refresh();			//	обновляет содержимое буфера
};

#endif
