#include <iostream>
#include <string.h>

/******************************************
*
*	пробелы считаются разделителями 
*	строк - надо настраивать cin
* 
*
*
*****************************/

#include "../net/socket.h"

#define B 1024

int main(){
	C_socket client;
	char buf[B];
	int p;

	cout << "Enter server addres : ";
	cin >> buf;
	cout << "Enter server port   : ";
	cin >> p;
	cout << "Connecting to server...";
	cout << (( client.connect(buf,p) == 0 ) ? "Ok" : "failed ") << endl;
	cout << "Sending your data ..." << endl;
	
	while (strcmp(buf,"quit")){
		cin >> buf;
		cout << "-=[" << buf << "]=-" << endl;
		client.write(buf,strlen(buf));
	}
	client.close();
	return 0;
}
