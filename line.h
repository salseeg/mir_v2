#ifndef __MIR__LINE
#define __MIR__LINE

//
//	����� - ������� ��� ������� � �������� �����
//	

#include "aux/ring_.h"

class C_line;

#include "connection.h"
#include "station.h"
#include "pulse_recognizer.h"

enum line_signal{			// ������� �����
	line_signal__line_deleted,	//	����������� ����� �� �������� ����������
	line_signal__line_added,	//	��������� ����� � ����������
	line_signal__line_dialed,	//	������� ����� ��������� ����� ������
	line_signal__holded_by,		//	������� ���� ����� ��������
	line_signal__unholded_by,	//	��������� ��������� �������� � ����������
	line_signal__holded_freed	//	���������� ���������� �����������
};
const int line_signal_count = 6;

enum line_stage {			// �������� ����(��) �����
	line_stage__free,		//	��������
	line_stage__ready,		//	����� - ������� ������� ������
	line_stage__digits,		//	����� - ������� ��������� �����
	line_stage__wait,		//	������� ����������
	line_stage__retranslation,	//	������� ���������� ������
	line_stage__connect,		//	� ����������
	line_stage__disconnected,	//	���������� ���������
	line_stage__incoming		//	�������� ���������� � ���������� (�����)
};
const int line_stage_count = 8;

enum line_type {		// ���� �����
	line_type__inner,	//	����������
	line_type__outer	//	�������
};

class C_line{
	private:
		char * last_number;
	protected:
		int id;					//	����� ����� (�� �������)
		
		typedef void (C_line::* stage_handler)();
		static stage_handler stage_handlers[line_stage_count];
		
		virtual void stage__free()=0;
		virtual void stage__ready()=0;
		virtual void stage__digits()=0;
		virtual void stage__wait()=0;
		virtual void stage__retranslation()=0;
		virtual void stage__connect()=0;
		virtual void stage__disconnect()=0;
		virtual void stage__incoming()=0;
		
		typedef void (C_line::* signal_handler)();
		static signal_handler signal_handlers[line_signal_count];

		virtual void signal__line_deleted()=0;
		virtual void signal__line_added()=0;
		virtual void signal__line_dialed()=0;
		virtual void signal__holded_by()=0;
		virtual void signal__unholded_by()=0;
		virtual void signal__holded_freed()=0;
		
	public:
		line_stage stage;			//	������ ��
		C_connection * current_connection;	//	������� ����������
		C_connection * incoming_connection;	//	
		int client_id;				//	������� ��������
		int own_bus_id;				//	����������� �����
		int operator_bus_id;			//	����� ���������
		int owner_id;				//	�� ����� ����������������� ��� (-1 = ��� ���������)
		bool block_incoming;			//	���������� �������� ����������
		bool block_notify;			//	���������� �����������
		bool blocked;				//	���������� ��� ��������� �������������� ��������� � �����
		
		C_pulse_recognizer * recognizer;
		C_ring_<C_connection> hold_ring;	//	���������� ����������
		
		C_line(int id);
		virtual ~C_line();
		
		int get_id();				//	������ ����� �����
		virtual int execute();			//	��������� ��������� �����

		virtual void remember_number();
		virtual void restore_number();
		char * get_last_number(){return last_number;}
		
		int operator==(const C_line& b){ return id == b.id;}
		int operator!=(const C_line& b){ return id != b.id;}

		virtual bool active()=0;		//	�������� ����� (��������� ���������(��������) � ����������)
					
		void signal(line_signal sig);
};

#endif 
// 	
