#include <mir/config.h>
#include <mir/hard.h>
#include <mir/melodies.h>

#include <iostream>

int main(){
	if (configure("mir.conf.xml")) return -1;
	unsigned long time = 0;
	while(1){
		melody_state ms = (*Melodies)[melody__got_connected]->get_state(time);
		if (ms != melody_state__error) {
			on_off st;
			if (ms == melody_state__low) 
				st = off;
			else 
				st = on;
			Hard->set_inner(0, inner_key__pv, st);
			cout << ((ms)? '#':'_');
			cout.flush();
		}
		
		Hard->sleep_us();
		time += Hard->get_delta();
	}
}
