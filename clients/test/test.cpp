#include "../../net/socket.h"
#include "../../interface/plugin.h"
#include "../../log/log.h"
#include <unistd.h>

#include <iostream>
#include <string.h>
#define B 1024

C_socket client;
C_service * service;

char * s_i = "test.so:Тест плагинов";
char * n_p = "new path";


/*
char * buf = new char[B];
int buf_len = B;

void sc_write(service_command command, C_socket * sock){
	sock->write(&(command.id), sizeof(command.id));
	sock->write(&(command.data_size), sizeof(command.data_size));
	sock->write(command.data, command.data_size);
}

service_command * sc_read(C_socket * sock){
	service_command * command;
	long * size = (long *)(buf + sizeof(command->id));
	int n = sizeof(command->id) + sizeof(command->data_size);

	if (*size + n > buf_len) return NULL;
	command = new struct service_command;
	::memcpy(command, buf, n);
	command->data = new char[command->data_size];
	::memcpy(command->data, buf + n, command->data_size);
	
	n += command->data_size;
	
	char * p = buf;
	if (buf_len > n) buf = new char[buf_len - n];
	else buf = NULL;
	buf_len -= n;
	::memcpy(buf, p, buf_len);
	delete [] p;
	
	return command;

}
*/
void parse_command(service_command * cmd){
	cout << "\ngot something to parse [ " << cmd->id << ":" << cmd->data_size <<" ]\n";
	cout.flush();
	switch (cmd->id){
		case 0x0002: cout << "Plugins path : " << (char *) cmd->data;
			     break;
		case 0xffff: client.close();
			     break;
	}
}


int main(){
	char buf[B];
	int p;
	
	Log = new C_log("client");
	
	cout << "Enter server addres : ";
	cin >> buf;
	cout << "Enter server port   : ";
	cin >> p;
	cout << "Connecting to server...";
	cout << (( client.connect(buf,p) == 0 ) ? "Ok" : "failed ") << endl;
	
	service = new C_service(&client);
	cout << 1;
	service_command *cmd;

	cmd = new service_command;
	cmd->id = 0x0000;
	cmd->data_size = ::strlen(s_i) + 1;
	cmd->data = new char[cmd->data_size];
	::strcpy((char *) cmd->data, s_i);
	cout << 111;
	service->write(*cmd);
	cout << "first command";
	delete[] (char *) cmd->data;
	cout.flush();
	cmd->id++;
	cmd->data_size = 0;
	cmd->data = NULL;
	service->write(*cmd);
	delete cmd;
	do{
		service->buf_refresh();
		cmd = service->read();
	}while(!cmd);
	parse_command(cmd);
	delete cmd;
	cmd = new service_command;
	cmd->id = 0x0002;
	cmd->data_size = ::strlen(n_p) + 1;
	cmd->data = new char[cmd->data_size];
	::strcpy((char *) cmd->data, n_p);
	service->write(*cmd);
	delete[] (char *) cmd->data;
	cmd->id--;
	cmd->data_size = 0;
	cmd->data = NULL;
	service->write(*cmd);
	delete cmd;
	do{
		service->buf_refresh();
		cmd = service->read();
		sleep(1);
		
	}while(!cmd);
	parse_command(cmd);
	delete cmd;
	
	while (1){
		do{
			service->buf_refresh();
			cmd = service->read();
			sleep(1);
			cout << ".";
			cout.flush();
		}while(!cmd);
		parse_command(cmd);
		delete cmd;
		sleep(1);
		cout << ".";
		cout.flush();
	}
	client.close();
	delete Log;
	return 0;
}
