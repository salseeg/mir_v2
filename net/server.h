#ifndef __MIR__SERVER
#define __MIR__SERVER

//
//	Сервер входящих соединений
//	Обслуживает пульты, мониторы, и т.п 
//	

#include <mir/net/socket.h>
#include <mir/net/service.h>
#include <mir/aux/ring_.h>

extern int server_max_connections;	//	максимальное количество соединений

class C_server{
	private:
		C_socket sock;			//	сокет
		C_ring_<C_service> services;	//	кольцо сервисов
		
		int check();			//	проверка входящих
		int work();			//	обслуживание подключенных
	public:
		C_server(int port);
		~C_server();
		int execute();			//	обработка
};

extern C_server * Server;

#endif
