#include "../../interface/plugin.h"
#include "../../log/log.h"
#include <iostream>

//extern char * plugins_path;

extern "C" int execute(C_service * service){
	service_command * command = service->read();
	if (!command) return 0;
	cout << "+";
	cout.flush();
	switch (command->id){
		case 0xfffe:{
				cout << "Got Error : " << (char *) command->data << endl;
			}break;
		case 0xffff:{
				Log->set_priority(log_priority__info);
				Log->rec() << "plugin got 0xffff";
				Log->write();
				return -1;
			}break;
		case 0x0001:{	// get_plugins_path
				service_command cmd;
				cmd.id = 0x0002;
				cmd.data_size = ::strlen(plugins_path) + 1;
				cmd.data = plugins_path;
				service->write(cmd);
			}break;
		case 0x0002:{	// set_plugins_path
				if (plugins_path) delete plugins_path;
				plugins_path = new char[command->data_size];
				::strcpy(plugins_path,(char *) command->data);
			}break; 	
	}
	delete command;
	return 0;
}
