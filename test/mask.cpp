
#include <mir/pulse_recognizer.h>
#include <mir/config.h>
#include <mir/log/log.h>
#include <mir/station.h>
#include <mir/masks.h>

#include <iostream.h>

int main(){
	if (configure("mir.conf.xml")) return -1;
	char s[40] = {0};
	while(strcmp(" ",s)){
		cout << "Mask : ";
		cin >> s;
		Station->lines[0]->recognizer = new C_pulse_recognizer(s);
		int n = Masks->ring.quantity();
		for (int i = 0; i < n; i++,Masks->ring.roll(1)){
			mask_check cr =	Masks->ring.get()->check(0);
			switch (cr){
				case mask_check__not_allowed:
					cout << "Not allowed" << endl;
					break;
				case mask_check__not_completed:
					cout << "Not completed" << endl;
					break;
				case mask_check__not_match:
					cout << "Not match" << endl;
					break;
				case mask_check__completed:
					cout << "Completed" << endl;
					break;
			}
		}
		delete Station->lines[0]->recognizer;	
	}
}
