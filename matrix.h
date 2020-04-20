#ifndef __MIR__MATRIX
#define __MIR__MATRIX

//
//	Матрица комутации. Интерфейс.
//

class C_matrix{
	public:
		virtual void connect(int connection_id, int line_id) = 0;	//	проключить точку
		virtual void disconnect(int conection_id, int line_id) = 0;	//	отключить точку
};

#endif
