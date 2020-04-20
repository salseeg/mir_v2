#include "file_sender.h"

#include <sys/types.h>		// 	+
#include <sys/stat.h>		//	|  open 
#include <fcntl.h>		//	+	
#include <unistd.h>		//	read, write, close

long file_sender__packet_size = 10000;

C_file_sender::C_file_sender(short cmd, char * filename) throw (char *){
	complete = false;
	command_id = cmd;
	buf = new char[file_sender__packet_size];

	fd = open(filename, O_RDONLY);

	if (fd < 0) 
		throw("Не могу открыть файл!");
}

C_file_sender::~C_file_sender(){
	close(fd);
	delete buf;
}

bool C_file_sender::check_complete(){
	return complete;
}

void C_file_sender::send(C_socket * socket){
	long size = read(fd, buf, file_sender__packet_size);
	if (size < file_sender__packet_size) 
		complete = true; 
	socket->write(&command_id, sizeof(command_id));
	socket->write(&size, sizeof(size));
	socket->write(buf, size);
}
