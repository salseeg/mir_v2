#include <mir/net/socket.h>
/**********************************************
*
*	TODO: errors handling
*
*
*
*
***********************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>			// non_blocking
#include <sys/time.h>

C_socket::C_socket(){
	other_side.name = NULL;
	other_side.port = 0;
	sock_id = ::socket(PF_INET, SOCK_STREAM, 0);
	state = sock_state__free;
}


C_socket::C_socket(int sock, inet_address os){
	sock_id = sock;
	state = sock_state__connected;
	other_side.name = new char[::strlen(os.name)+1];
	other_side.name = ::strcpy(other_side.name,os.name);
	other_side.port = os.port;
}


C_socket::~C_socket(){
	close();
	if (other_side.name != NULL) delete [] other_side.name;
}


int C_socket::connect(char * server_name, int server_port){
	struct sockaddr_in *addr;
	int result;

	if (state != sock_state__free) return -1;
	addr = new struct sockaddr_in;
	addr->sin_family = AF_INET;
	::inet_aton(server_name, &(addr->sin_addr));
	addr->sin_port = htons(server_port);
	result = ::connect(sock_id,(struct sockaddr *) addr, sizeof(struct sockaddr_in));
	delete addr;
	if (result == 0) {
		state = sock_state__connected;
		other_side.name = new char[::strlen(server_name)+1];
		other_side.name = ::strcpy(other_side.name, server_name);
		other_side.port = server_port;
	}
	return result;
}


int C_socket::bind(int port){
	struct sockaddr_in * addr;
	int result;
	
	if (state != sock_state__free) return -1;
	
	addr = new struct sockaddr_in;
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(port);
	result = ::bind(sock_id,(struct sockaddr *) addr, sizeof(struct sockaddr_in));
	delete addr;
	if (result == 0) state = sock_state__binded;
	return result;
}


int C_socket::listen(int queue_size){
	int result;
	
	if (state != sock_state__binded) return -1;
	result = ::listen(sock_id, queue_size);
	if (result == 0) state = sock_state__listened;
	return result;
}


C_socket * C_socket::accept(){
	int result;
	struct sockaddr_in * addr;
	char * name;		// other side address name
	int size;		// address size
	C_socket * ns = NULL;	// new socket
	struct inet_address * os = NULL;

	if (state != sock_state__listened) return NULL;
	addr = new struct sockaddr_in;
	size =  sizeof(struct sockaddr_in);
	result = ::accept(sock_id,(struct sockaddr *) addr,(socklen_t *) &size);
	if (result > 0){
		os = new struct inet_address;
		name = inet_ntoa(addr->sin_addr);
		os->name = new char[::strlen(name) + 1];
		os->name = ::strcpy(os->name,name);
		os->port = addr->sin_port; 
		ns = new C_socket(result, *os);
		delete os->name;
		delete os;
	}
	delete addr;
	return (result > 0) ? ns : NULL;
}


int C_socket::read(void * buf, int len){
	if (state != sock_state__connected) return -1;	
	return ::recv(sock_id, buf, len, 0);
}

int C_socket::write(const void * buf, int len){
	if (state != sock_state__connected) return -1;
	return ::send(sock_id, buf, len, 0);
}

int C_socket::close(){
	int result;
	
	if (state != sock_state__connected && state != sock_state__listened ) return -1;
	shutdown(sock_id, SHUT_RDWR);
	result = ::close(sock_id);
	if (result == 0) state = sock_state__free;
	return result;
}


inet_address * C_socket::get_other_side(){
	return &other_side;
}

int C_socket::set_blocking(sock_blocking b){
	int options;

	options = fcntl(sock_id, F_GETFL);
	if (b == sock_blocking__off) options |= O_NONBLOCK;
	if (b == sock_blocking__on) options &= ~O_NONBLOCK;
	fcntl(sock_id, F_SETFL, options);
	
	return 0;
}
