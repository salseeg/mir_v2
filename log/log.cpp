#include "log.h"
#include <syslog.h>
#include <string.h>

const char type[8][20] = {"[ Emergency ]",
			"[ Alert ]",
			"[ Critical ]",
			"[ Error ]",
			"[ Warning ]",
			"[ Notice ]",
			"[ Info ]",
			"[ Debug ]"};


C_log::C_log(char * appname){
	::openlog(appname, LOG_NDELAY, LOG_LOCAL7);
	
	record = new ostrstream(buf, LOG_BUF_LENGTH);
	set_priority(log_priority__debug);	
	*record << "Creating logger ...";
	write();
}


ostrstream& C_log::rec(){
	return *record;
}


void C_log::write(){
	::syslog(priority, "%s %s",type[(int) priority], record->str());
	delete record;
	::memset(buf, '\0', LOG_BUF_LENGTH);
	record = new ostrstream(buf, LOG_BUF_LENGTH);
}


void C_log::set_priority(enum log_priority pr){
	if (*buf) write();	// flush log
	priority = pr;
}


C_log::~C_log(){
	set_priority(log_priority__debug);
	*record << "Destroying logger ...";
	write();
	::closelog();
	delete record;
}

C_log * Log;
