#ifndef __MIR__SWITCHER
#define __MIR__SWITCHER

#include <mir/player.h>
#include <mir/hard.h>

class C_switcher{
	private:
		int n_inner;
		int n_outer;
		C_player *** inner;
		C_player *** outer;
	public:
		C_switcher(int n_i, int n_o);
		~C_switcher();
		
		void execute(unsigned long dtime);
		C_player & player(int line, inner_key key);
		C_player & player(int line, outer_key key);	
};
#endif
