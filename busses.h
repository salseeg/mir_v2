#ifndef __MIR__BUSSES
#define __MIR__BUSSES

#include "bus.h"
#include "aux/ring_.h"

class C_busses{
	public:
		C_ring_<C_bus> ring;

		~C_busses();
		C_bus * get_bus(int bus_id);
};

extern C_busses * Busses;

#endif
