#ifndef __MIR__FILE_SENDER
#define __MIR__FILE_SENDER

//	Отправляет файл частями через сокет
//	для отправки очередной части вызывается send
//	для проверки завершения отправки - check_complete

#include <mir/net/socket.h>

class C_file_sender{
	private:
		int fd;
		char * buf;
		bool complete;
		short command_id;
	public:
		C_file_sender(short cmd, char * file_name) throw (char *);
		~C_file_sender();

		void send(C_socket * socket);
		bool check_complete();
};

extern long file_sender__packet_size;

#endif
