#ifndef __MIR__MATRIX
#define __MIR__MATRIX

//
//	������� ���������. ���������.
//

class C_matrix{
	public:
		virtual void connect(int connection_id, int line_id) = 0;	//	���������� �����
		virtual void disconnect(int conection_id, int line_id) = 0;	//	��������� �����
};

#endif
