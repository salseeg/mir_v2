#include "server.h"

int server_max_connections = 32;
C_server * Server;

C_server::C_server(int port){
	sock.bind(port);
	sock.listen(server_max_connections);
	sock.set_blocking(sock_blocking__off);
}

C_server::~C_server(){
	int n;
	n = services.quantity();
	for (int i = 0; i < n; i++){
		delete services.pop();
	}
}

int C_server::check(){
	C_socket * s;
	do{
		s = sock.accept();
		if (s){
			services.add(new C_service(s));
		}	
	}while (s);
	
	return 0;
}

//==============================================================
//  Проход по всем сервисам
//  и реакция на команды
//  	-1   отключение сервиса
//  	-2   завершение работы станции
int C_server::work(){
	int n;
	bool exit_flag = false;
	n = services.quantity();
	for (int i = 0; i < n; i++){
		int exit_code = services.get()->execute();
		if (exit_code == -1) {
			delete services.pop();
		}else if (exit_code == -2){
			exit_flag = true;
			services.roll(1);
		}else {
			services.roll(1);
		}
	}
	if (exit_flag){	// разрыв соединений
		int n = services.quantity();
		for (int i = 0; i < n; i++){
			delete services.pop();
		}
	}
	return exit_flag ? -1 : 0;
}


int C_server::execute(){
	check();
	return work();
}
