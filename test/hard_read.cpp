#include "../hard.h"
#include "../pulse_recognizer.h"
#include "../config.h"
#include "../log/log.h"

#include <iostream.h>

int main(){
	if (configure("mir.conf.xml")) return -1;
	C_pulse_recognizer pr;
	
	while(1){
		bit_history bh;
		bh = Hard->get_state_inner(0);
		
			
		recognition_result rr =	pr.recognize(bh);
		switch (rr){
			case recognition_result__error:{
				cout << "!";
				break;
			}	
			case recognition_result__not_started:{
				cout << "_";
				break;
			}
			case recognition_result__not_ended:{
				cout << ".";
				break;
			}
			case recognition_result__digit:{
				int n = pr.get_length();
				cout << "\n";
				for (int i = 0; i < n; i++){
					cout << ((int) pr[i]);
				}
				cout << "\n";
				break;					
			}							     
		}
		cout.flush();
		Hard->sleep_us();
	}
}
