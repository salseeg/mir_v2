#include <mir/config.h>
#include <mir/log/log.h>

int main(){
	configure("mir.conf.xml");
	if (Log) delete Log;
}
