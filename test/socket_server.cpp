#include <iostream>
#include "../net/socket.h"
#include <unistd.h>

/**************************************************
*
*	Разрыв соединения на прикладном уровне или 
*	обработка сигналов приложения, что не 
*	подходит для компонентной модели :(
*
*
*
* 
**********************************/

#define S 3
#define B 1024

C_socket server;
C_socket * clients[S];

int main(){
	int i, j = 0, c;
	char buf[B];

	for (i = 0; i < S; i++) clients[i] = NULL;
	
	cout << "Binding server to 4444 port\t\t\t";
	cout << ((server.bind(4444) == 0) ? "Ok" : "failed") << endl;
	cout << "Switching to listenning mode\t\t\t";
	cout << ((server.listen(4) == 0) ? "Ok" : "failed") << endl;
	cout << "Accepting new connections..." << endl;
	server.set_blocking(sock_blocking__off);	
	while(j <= S){
		clients[j] = server.accept();
		if (clients[j] != NULL){
			cout << "\nNew connection accepted [" << j << "]\t";
			cout << "\t" << clients[j]->get_other_side()->name << " : ";
			cout << clients[j]->get_other_side()->port << endl;
			j++;
		}
		for (i = 0; i < j; i++){
			do{
				clients[i]->set_blocking(sock_blocking__off);
				c = clients[i]->read(buf,B);
				clients[i]->set_blocking(sock_blocking__on);
				buf[c] = 0;
				if (c > 0) cout << "\n[" << i << "] " << c << " : " << buf << endl;
			}while (c > 0);
		};
		cout << ".";
		cout.flush();
		sleep(1);
	}
	return 0;
}
