#include "../../interface/plugin.h"
#include "../../log/log.h"
#include "../../station.h"

#include <stdio.h>
#include <string.h>
#include "../../aux/ring_.h"
#include "file_sender.h"

#define BUF_SIZE 1024 << 1

//  :::::: Команды плагину ::::::::
//  0x0001	Получить состояния линий		0x0001
//  0x0002	Получить состояния соединений		0x0002
//  0x0003	Получить файл конфигурации		0x0003
//  0x0004	Получить файл журнала			0x0004
//  0x0005	Загрузить файл конфигурации		 ----
//  0x0006	Загрузить плагин			 ----
//  0x0007	Выключить программу (перезапуск)	0xffff
//  0x0008	Выключить станцию (poweroff)		0xffff

char * stage_names[] = {
	"Свободен",
	"Готов",
	"Набор номера",
	"Ожидание",
	"Ретрансляция",
	"Соединение",
	"Занято",
	"Входящий"
};

char * get_line_states(){
	char buf[BUF_SIZE] = {0};
	char * s = buf;
	int n = Station->lines;
	for (int i = 0; i < n; i++){
		C_line * l = Station->lines[i];
		int stage = (int) l->stage;
		int cur_con = (l->current_connection) ?
			l->current_connection->get_id() : -1;
		int inc_con = (l->incoming_connection) ?
			l->incoming_connection->get_id() : -1;
		char * rec_num = (l->recognizer) ? ((char *)(*(l->recognizer))) : NULL;
		int o_id = l->owner_id;
		while(*s) s++;
		sprintf(s, "Line %2d : %s : %2d : %2d : %s : %2d\n", 
				i,			// Номер линии
				stage_names[stage],	// Стадия жизненого цикла
				cur_con, 		// Текущее соединение
				inc_con, 		// Входящее соединение
				rec_num,		// Распознанный номер
				o_id);			// Номер владельца линии
	}
	char * r = new char[::strlen(buf) + 1];
	::strcpy(r, buf);
	return r;
}

char * get_connections_states(){
	char buf[BUF_SIZE] = {0};
	char * s = buf;
	int n = Station->connections.quantity();
	for (int i = 0; i < n; i++, Station->connections.roll()){
		C_connection * con = Station->connections.get();
		int id = con->get_id();
		while(*s) s++;
		sprintf(s, "Connection %2d :", id);
		int m = con->lines.quantity();
		for (int j = 0; j < m; j++, con->lines.roll()){
			int id = con->lines.get()->get_id();
			while(*s) s++;
			sprintf(s," %d", id);
		}
		while(*s) s++;
		sprintf(s,"\n");
	}
	n = Station->free_connections.quantity();
	for (int i = 0; i < n; i++, Station->free_connections.roll()){
		int id = Station->free_connections.get()->get_id();
		while(*s) s++;
		sprintf(s, "Connection %2d : free\n", id);
	}
	char * r = new char[::strlen(buf) + 1];
	::strcpy(r, buf);
	return r;
}

C_ring_<C_file_sender> senders;

extern "C" int execute(C_service * service){
	int n = senders.quantity();
	for (int i = 0; i < n; i++, senders.roll()){
		C_file_sender * sndr = senders.get();
		if (sndr->check_complete()){
			delete senders.pop();
			senders.roll(-1);
		}else{
			sndr->send(service->sock);
		}
	}
	
	C_service_command * command = service->read();
	if (!command) return 0;
	switch (command->id){
		case 0xfffe:{
				struct inet_address * os_addr = service->sock->get_other_side();
				Log->set_priority(log_priority__error);
				Log->rec() << "Ошибка со стороны клиента [";
				Log->rec() << os_addr->name << ":" << os_addr->port;
				Log->rec() << "] : " << command->get_data() << endl;
				Log->write();
			}break;
		case 0xffff:{
				struct inet_address * os_addr = service->sock->get_other_side();
				Log->set_priority(log_priority__info);
				Log->rec() << "Разрыв соединения [";
				Log->rec() << os_addr->name << ":" << os_addr->port;
				Log->rec() << "]" << endl;
				Log->write();				
				return -1;
			}break;
		case 0x0001:{	// get_line_stages
				char * s = get_line_states();
				C_service_command cmd(0x0001, ::strlen(s) + 1, s);
				service->write(cmd);
				delete s;
			}break;
		case 0x0002:{	// get_connections_states
				char * s = get_connections_states();
				C_service_command cmd(0x0002, ::strlen(s) + 1, s);
				service->write(cmd);
				delete s;
			}break;
		case 0x0003:{
				C_file_sender * sndr;
				try{
					sndr = new C_file_sender(0x0003, "/usr/share/mir/mir.conf.xml");
				}catch (char * err_str){
					C_service_command cmd(0xfffe, ::strlen(err_str) + 1, err_str);
					service->write(cmd);
					return 0;
				}
				senders.add(sndr);
			}break;
		case 0x0004:{
			}break;
		case 0x0005:{
			}break;
		case 0x0006:{
			}break;
		case 0x0007:{
				struct inet_address * os_addr = service->sock->get_other_side();
				Log->set_priority(log_priority__info);
				Log->rec() << "Завершение программы обработки [";
				Log->rec() << os_addr->name << ":" << os_addr->port;
				Log->rec() << "]" << endl;
				Log->write();				
				return -2;

			}break;
	}
	delete command;
	return 0;
}
