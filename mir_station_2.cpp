#include "config.h"
#include "hard.h"
#include "masks.h"
#include "melodies.h"
#include "net/server.h"
#include "station.h"
#include "log/log.h"
#include "busses.h"

#include <unistd.h>


//для работы с железом закоментировать 
//#define NO_HARD_SLEEP

#define CONF_FILE_NAME "/usr/share/mir/mir.conf.xml"

int init(char * conf_file_name){
	return configure(conf_file_name);
}

void done(){
	if (Hard) delete Hard;
	if (Masks) delete Masks;
	if (Server) delete Server;
	if (Melodies) delete Melodies;
	if (Busses) delete Busses;
	if (Station) delete Station;
	if (Log) delete Log;
	
}

int main_loop(){
	int exi = 0;
	while (!exi) {
#ifndef NO_HARD_SLEEP
		Hard->sleep_us();
#else
		usleep(20000);
#endif
		if ((exi = Station->execute())) break;
		if ((exi = Server->execute())) break;
	}
	return exi;
}

int main(){
	int res = 0;
	if (!init(CONF_FILE_NAME)){
		res = main_loop();
	}
	done();	
	return res;
}
