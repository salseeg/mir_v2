#ifndef __MIR__SERVICE
#define __MIR__SERVICE
/***************************************************************
*	
*	������ ��������� �������������� ����� ��������
*	� ��������(��������) ������ ���������� service_command
*		unsigned short id       - ������������� ������� 
*		long data_size		- ������ ������
*		char data[data_size]	- ������
*	
*
*	���������������� �������������� ������
*	�������:
*		0x0000	-  �����������
*			������ ���� ������ ��������, ������������ 
*			�������, ����� ������ ����������
*	�������:
*		0xfffe  -  ��������� �� ������
*		0xffff  -  ��������� ����������
*
*	������ ������ ������ ��������� ����� ����� 
*		int execute(C_service * service);
*	���������� -1 ��� ������� ����������.
*		   -2  - ���������� �������.
*
****************************************/

#include "socket.h"
#include <stdlib.h>

extern char * plugins_path;	//	���� � ��������


class C_service_command{	//  ������� �������:
	private:
		char * data;		//	������	
	public:
		unsigned short id;	//	������������� �������
		long data_size;		//	������ ������
			
		void set_data(const char *, long); 
		const char * get_data() const;
		
		C_service_command();
		C_service_command(unsigned short id_, long ds_, char * data_);
		C_service_command(const C_service_command & b);
		C_service_command& operator=(const C_service_command &b);
		~C_service_command();
};


class C_service{
	private:
		char * buf;			//	�����
		int buf_len;			//	����� ������
		int (* plugin)(C_service *);	//	���������� �������
		void * plugin_handle;		//	����� �������
	public:
		C_socket * sock;		//	�����
		char * service_name;		//	�������� �������
		
		C_service(C_socket * s);
		~C_service();
		C_service_command * read();		//	���������� �������� �������, NULL - �����
		void write(const C_service_command & command);	//	���������� �������
		int execute();				//	���������
		void buf_refresh();			//	��������� ���������� ������
};

#endif
