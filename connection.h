#ifndef __MIR__CONNECTION
#define __MIR__CONNECTION
//
//	Соединение
//

class C_connection;

#include <mir/aux/ring_.h>
#include <mir/line.h>

class C_connection{
	private:
		int id;				//	номер соединения
		bool lock;			//	блокировка размножение сигналов удаления
		void free();			//	освободить соединение
	public:
		C_connection(int id);
		~C_connection();
		
		C_ring_<C_line> lines;		//	линии
		C_ring_<C_line> holded_by;	//	список линий которые удерживают данную линию
		
		void add_line(C_line & line);	//	подключить линию к соединению
		void del_line(C_line & line);	//	отключить линию от соединения

		void silent_add_line(C_line & line);
		void silent_del_line(C_line & line);

		void move_line(C_line& line, C_connection& dst_conection); 	// перевести линию из данного соединения в назначенное
		void mix(C_connection& other_connection);			// объединить два соединения 
		void mix_without(C_line& line, C_connection& other_connection);	// объединить два соединения без инициатора

		void hold(C_line & line);	//	удержать соединение линией
		void unhold(C_line & line);	//	вернуться линией к удержаному ранее соединению

		int get_id(){return id;}					// вернуть идентификатор соединения
		int operator==(const C_connection& b){return id == b.id;}	// сравнить с соединением
};

#endif
