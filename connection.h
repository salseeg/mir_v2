#ifndef __MIR__CONNECTION
#define __MIR__CONNECTION
//
//	����������
//

class C_connection;

#include <mir/aux/ring_.h>
#include <mir/line.h>

class C_connection{
	private:
		int id;				//	����� ����������
		bool lock;			//	���������� ����������� �������� ��������
		void free();			//	���������� ����������
	public:
		C_connection(int id);
		~C_connection();
		
		C_ring_<C_line> lines;		//	�����
		C_ring_<C_line> holded_by;	//	������ ����� ������� ���������� ������ �����
		
		void add_line(C_line & line);	//	���������� ����� � ����������
		void del_line(C_line & line);	//	��������� ����� �� ����������

		void silent_add_line(C_line & line);
		void silent_del_line(C_line & line);

		void move_line(C_line& line, C_connection& dst_conection); 	// ��������� ����� �� ������� ���������� � �����������
		void mix(C_connection& other_connection);			// ���������� ��� ���������� 
		void mix_without(C_line& line, C_connection& other_connection);	// ���������� ��� ���������� ��� ����������

		void hold(C_line & line);	//	�������� ���������� ������
		void unhold(C_line & line);	//	��������� ������ � ���������� ����� ����������

		int get_id(){return id;}					// ������� ������������� ����������
		int operator==(const C_connection& b){return id == b.id;}	// �������� � �����������
};

#endif
