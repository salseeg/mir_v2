#ifndef __MIR__SOCKET
#define __MIR__SOCKET
//
//	�����
//

enum sock_state{		// ��������� ������:
	sock_state__free,	// 	��������
	sock_state__connected,	// 	���������
	sock_state__binded,	// 	��������
	sock_state__listened	// 	�������
};

enum sock_blocking{		// ����������
	sock_blocking__on,	//	�������
	sock_blocking__off	//	���������
};

struct inet_address{		// ����� ������ �������
	char * name;		//	��� (somwhere.net / 122.22.33.4)
	int port;		//	���� (21)
};

class C_socket{
	private:
		int sock_id;					// ����� ������
		sock_state state;				// ��������� ������
		inet_address other_side;			// ����� ������ �������
	public:
		C_socket();					// �������� ������
		C_socket(int sock_id,inet_address os);		// ����� ����������
		~C_socket();					// �������� ������
		int connect(char * server_name,int server_port);// ����������� ������ � ���������� �������
		int bind(int port);				// �������� ������ � ���������� �����
		C_socket * accept();				// ����� ������������ ����������
		int listen(int queue_size);			// ������������� ������ ������� ��� ����������� ����������
		int read(void * buf,int len);			// ������ ������
		int write(const void * buf,int len);		// ������ ������
		int close();					// ���������� ����������
		inet_address * get_other_side();		// ����� ���������� ������������ ����������
		int set_blocking(sock_blocking b);		// �������������/������� ����������
};
#endif
