#include <dlfcn.h>
#include <string.h>
#include "../log/log.h"
#include "service.h"
#include <iostream.h>


#define LOCAL_BUF 512


char * plugins_path;

C_service_command::C_service_command():
	data(NULL), id(0xffff), data_size(0){}

C_service_command::C_service_command(unsigned short id_, long ds_,
		char * d_):data(NULL),id(id_), data_size(ds_){
	if (d_) {
		data = new char[data_size];
		memcpy(data, d_, data_size);
	}
}

C_service_command & C_service_command::operator=(const C_service_command &b){
	if (this == &b) return *this;
	id = b.id;
	data_size = b.data_size;
	if (data) {
		delete data;
		data = NULL;
	}
	if (data_size){
		data = new char[data_size];
		memcpy(data, b.data, data_size);
	}
	return *this;
}

C_service_command::~C_service_command(){
	if (data){
		delete data;
	}
}

void C_service_command::set_data(const char * s, long ds_){
	if (data) {
		delete data;
		data = NULL;
		data_size = 0;
	}
	if (s){
		data = new char [ds_];
		memcpy(data, s, ds_);
		data_size = ds_;
	}
}

const char * C_service_command::get_data() const{
	return data;
}

C_service::C_service(C_socket * s){
	sock = s;
	service_name = NULL;
	plugin_handle = NULL;
	buf = NULL;
	buf_len = 0;


	inet_address * os = sock->get_other_side();
	Log->set_priority(log_priority__info);
	Log->rec() << "Установлено соединение с " << os->name << " : " << os->port;
	Log->write();
}

C_service::~C_service(){
	inet_address * os = sock->get_other_side();
	Log->set_priority(log_priority__info);
	Log->rec() << "Завершено соединение с " << os->name << " : " << os->port;
	Log->write();
	
	if (service_name) delete[] service_name;
	if (plugin_handle){
		// разорвать соединение с клиентом
		C_service_command command(0xffff, 0, NULL);
		write(command);

		if (! ::dlclose(plugin_handle)) plugin_handle = NULL;
	}
	if (buf) delete[] buf;	
	delete sock;
}

void C_service::buf_refresh(){
	char * b = new char[LOCAL_BUF];
	int i;

	sock->set_blocking(sock_blocking__off);
	i = sock->read(b, LOCAL_BUF);
	sock->set_blocking(sock_blocking__on);

	if (i > 0){
		char * p = buf;
		buf = new char[buf_len + i];
		if (p){
			::memcpy(buf , p, buf_len);
		}
		::memcpy(buf + buf_len, b, i);
		//for (int j = 0; j < i; j++){
		//	cout << hex << buf[buf_len + j];
		//}
		//cout << endl;
		if (p){
			delete[] p;
			p = NULL;
		}
		buf_len += i;
	}
	delete b;
}

void C_service::write(const C_service_command &command){
	sock->write(&(command.id), sizeof(command.id));
	sock->write(&(command.data_size), sizeof(command.data_size));
	sock->write(command.get_data(), command.data_size);
}

C_service_command * C_service::read(){
	C_service_command * command = NULL;

	int n = sizeof(short) + sizeof(long);
	if (buf_len < n) return NULL;
	long * size = (long *)(buf + sizeof(short));
	
	if (*size + n > buf_len) return NULL;
	command = new C_service_command;
	command->id = * (unsigned short *) buf;
	//command->data_size = *size;
	command->set_data(buf + n, *size);
	//command->data = new char[command->data_size];
	//::memcpy(command->data, buf + n, command->data_size);
	
	n += command->data_size;
	
	char * p = buf;
	buf_len -= n;
	if (buf_len > 0){
		buf = new char[buf_len];   // [buf_len - n] ???
		::memcpy(buf, p + n, buf_len);
	}else{
		buf = NULL;
	}
	delete [] p;
	
	return command;
}


int C_service::execute(){
	buf_refresh();
	
	if (!plugin_handle){
		C_service_command * command = read();
		
		if (!command) {
			//cout << "command == NULL" << endl;
			return 0;
		}
		if (command->id != 0x0000){
			cout << "command id = "<< command->id << endl;
			command->id = 0xffff;
			command->data_size = 0;
			write(*command);
			delete command;
			return -1;
		}
		// парсить строки , подключать плагин
		char * plugin_filename = new char[::strlen(plugins_path) + command->data_size + 1];
		char * p;
		char * str = new char[command->data_size];
		::strcpy (str, command->get_data());
		cout << str << endl;

		::strcpy(plugin_filename, plugins_path);
		p = ::strtok(str, ":");
		::strcat(plugin_filename, p);
		service_name = new char[::strlen(str) + 1];
		::strcpy(service_name, str);
		delete command;
		delete [] str;

		plugin_handle = dlopen(plugin_filename, RTLD_LAZY);
		if (!plugin_handle){
			p = dlerror();
			char * ppp = new char [::strlen(plugin_filename) + ::strlen(p) + 2];
			::strcpy(ppp, plugin_filename);
			::strcat(ppp, ":");
			::strcpy(ppp, p);
			command = new C_service_command(0xfffe, strlen(ppp) + 1, ppp);
			delete [] ppp;
			write(*command);
			(command->id)++;
			command->set_data(NULL, 0);
			write(*command);
			delete command;
			
			struct inet_address * os = sock->get_other_side();
			Log->set_priority(log_priority__warning);
			Log->rec() << "Плагин не найден : " << plugin_filename << " [";
			Log->rec() << os->name << " : " << os->port << "]";
			Log->write();

			delete [] plugin_filename;
			return -1;			
		}

		plugin = (int (*)(C_service *)) dlsym(plugin_handle, "execute");
		if (!plugin){
			p = dlerror();
			command = new C_service_command(0xfffe, strlen(p) + 1, p);
			write(*command);
			(command->id)++;
			command->set_data(NULL, 0);
			write(*command);
			delete command;

			Log->set_priority(log_priority__error);
			Log->rec() << "Не найдена входная точка 'execute()' в плагине " << plugin_filename;
			Log->write();

			delete [] plugin_filename;
			return -1;			
		}	
		delete [] plugin_filename;

		struct inet_address * os = sock->get_other_side();
		Log->set_priority(log_priority__info);
		Log->rec() << "Плагин : " << service_name << " [" <<os->name << " : ";
		Log->rec() << os->port <<  "]";
		Log->write();

	}else{
		return plugin(this);
	}
	return 0;
}
