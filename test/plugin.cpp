#include "../net/server.h"
#include "../log/log.h"
#include <unistd.h>
#include <iostream.h>

void init(){
	plugins_path = new char[60];
	::strcpy(plugins_path,"/usr/share/mir/plugins/");

	Log = new C_log("plugin_test");
	Server = new C_server(4444);
}

void work(){
	while (1){
		Server->execute();
		sleep(1);
		cout << ".";
		cout.flush();
	}
}


int main(){
	init();
	work();
	return 0;
}
