#include "../interface/driver.h"
#include "../pulse_recognizer.h"

#include <iostream>		//	cout
#include <sys/types.h>		// +
#include <sys/stat.h>		// |	open	
#include <fcntl.h>		// +
#include <unistd.h>		//	read, write. close

#define N_INNER 8

int main(){
	C_pulse_recognizer recognizer;
	int dev_fd = open("/dev/mir", O_RDWR);
	bit_history bits[N_INNER];
	read_line_states states;

	states.quantity = N_INNER;
	states.line_type = LT_inner;
	states.bits = bits;

	while (1){
		read(dev_fd, &states, sizeof(states));
		recognition_result res = recognizer.recognize(bits[0]);
		switch (res){
			case recognition_result__error :{
				cout << "!";
				break;
			}
			case recognition_result__not_ended :{
				cout << ".";
				break;
			}
			case recognition_result__not_started : {
				cout << "_";
				break;
			}
			case recognition_result__digit:{
				for (int i = 0; i < recognizer.get_length(); i++){
					cout << (int) recognizer[i];
				}
				cout << "\n";
				break;
			}			
		}
		cout.flush();
		write(dev_fd,NULL,0);
	}
	close(dev_fd);
}
