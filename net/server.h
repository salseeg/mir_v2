#ifndef __MIR__SERVER
#define __MIR__SERVER

//
//	������ �������� ����������
//	����������� ������, ��������, � �.� 
//	

#include <mir/net/socket.h>
#include <mir/net/service.h>
#include <mir/aux/ring_.h>

extern int server_max_connections;	//	������������ ���������� ����������

class C_server{
	private:
		C_socket sock;			//	�����
		C_ring_<C_service> services;	//	������ ��������
		
		int check();			//	�������� ��������
		int work();			//	������������ ������������
	public:
		C_server(int port);
		~C_server();
		int execute();			//	���������
};

extern C_server * Server;

#endif
