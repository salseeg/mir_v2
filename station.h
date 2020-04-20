#ifndef __MIR__STATION
#define __MIR__STATION

#include "aux/ring_.h"
#include "aux/array_.h"

class C_station;

#include "connection.h"
#include "line.h"
#include "switcher.h"

class C_station{
	private:
		C_connection * music_connection;
		int n_i, n_o;
	public:
		C_station(int n_i_, int n_o_);			
		~C_station();
		
		C_array_<C_line> lines;				//	линии
		C_ring_<C_connection> connections;		//	используемые шнурки
		C_ring_<C_connection> free_connections;		//	свободные шнурки
		C_switcher switcher;

		
		C_connection * get_free_connection();
		void free_connection(C_connection * con);

		void add_music(C_line& ab);
		void del_music(C_line& ab);
		
		int execute();

		int n_in();
		int n_out();
};

extern C_station * Station;

#endif
