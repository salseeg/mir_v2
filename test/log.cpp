#include <mir/log/log.h>

int main(){
	Log = new C_log("log_test");	

	int i = 124;
	double d = -8.008;
	char * str = "some dumb string";

	Log->set_priority(log_priority__alert);
	Log->rec() << "Passing int : " << i;
	Log->write();

	Log->set_priority(log_priority__info);
	Log->rec() << "Passing double : ";
	Log->rec() << d;
	Log->write();

	Log->set_priority(log_priority__warning);
	Log->rec() << "Passing string :" << str;
	Log->write();

	delete Log;
	return 0;
}
