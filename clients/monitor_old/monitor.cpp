#include "../../interface/plugin.h"
#include "../../log/log.h"
#include <unistd.h>

#include <iostream>
#include <string.h>
#define B 1024

#define LOCAL_SERVER

C_socket * client;
C_service * service;

char * s_i = "monitor.so:Station monitor";

void send_command(int id, int size, char * p){
	C_service_command cmd(id, size, p);
	service->write(cmd);
}

typedef int (*handler)();

// insert handlers

int do_quit(){
	send_command(0xffff, 0, NULL);
	return -1;
}

int get_line_states(){
	send_command(0x0001, 0, NULL);
	return 0;
}

int get_connections_states(){
	send_command(0x0002, 0, NULL);
	return 0;
}

const int handlers_count = 3;
struct {
	char * str;
	handler f;
} handlers[handlers_count] = {
	{"quit", do_quit},
	{"line_states", get_line_states},
	{"connections", get_connections_states}
};

void parse_command(const C_service_command & cmd){
	cout << "\ngot something to parse [ " << cmd.id << ":" << cmd.data_size <<" ]\n";
	cout.flush();
	switch (cmd.id){
		case 0x0001: cout << "\n::: Line states :::\n" << cmd.get_data();
			     break;
		case 0x0002: cout << "\n::: Connections :::\n" << cmd.get_data();
			     break;
		case 0xffff: client->close();
			     break;
	}
}

int menu(char * s){
	handler f;
	for (int i = 0; i < handlers_count; i++){
		if (!strcmp(handlers[i].str, s)){
			f = handlers[i].f;
			return f();
		}
	}
	cout << "Unknown command!!!" << endl;
	return -2;
}
		
int init(){
	char buf[B];
	int p;
	
	Log = new C_log("client");

	client = new C_socket();

#ifndef LOCAL_SERVER	
	cout << "Enter server addres : ";
	cin >> buf;
	cout << "Enter server port   : ";
	cin >> p;
#else
	strcpy(buf,"127.0.0.1");
	p = 3003;
#endif
	
	cout << "Connecting to server...";
	int b = client->connect(buf,p);
	cout << ((!b) ? "Ok" : "failed ") << endl;
	if (b) {
		delete client;
		return -1;
	}
	
	service = new C_service(client);
	C_service_command cmd(0x0000 ,::strlen(s_i) + 1, s_i);
	service->write(cmd);
	return 0;
}


void main_loop(){
	bool quit = false;
	char *s = new char[B];
	char *prev = NULL;
	while (!quit){
		bool skip_rec = false;
		C_service_command * ans;
		cout << "\n: ";
		cout.flush();
		cin >> s;
		if (!strcmp("rep",s)){
			strcpy(s,prev);
		}
		int mr = menu(s);
		switch (mr){
			case -1:	// exit
				quit = true;
				break;
			case -2:	// unknown command
				skip_rec = true;
				break;
		}
		if (!skip_rec){
			do{	//	прием сообщения от сервера
				service->buf_refresh();
				ans = service->read();
				if (!ans){
					cout << "Sleep for a second .." << endl;
					sleep(1);
				}
			}while(!ans);
			parse_command(*ans);
			delete ans;
		}
		if (prev) delete prev;
		prev = s;
		s = new char[B];
	}	
}

void done(){
	if (service) delete service;
	if (client) delete client;
	delete Log;	
}

int main(){
	if (!init()){
		main_loop();
	}
	done();
}
