#ifndef __MIR__BUS
#define __MIR__BUS

#include <mir/line.h>
#include <mir/aux/ring_.h>

class C_bus{
	private:		
		int bus_id;
		bool roll;
	public:
		C_bus(int id, bool roll_ = true);
		~C_bus();
		
		C_ring_<C_line> lines;
		
		C_line * get_free_line(int l_id = -1);
		int get_free_line_id(int l_id = -1);
		
		int get_bus_id();
};

#endif
