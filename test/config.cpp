#include "../config.h"
#include "../log/log.h"

int main(){
	configure("mir.conf.xml");
	if (Log) delete Log;
}
