
#ifndef __MIR__OUTER_LINE
#define __MIR__OUTER_LINE

#include <mir/line.h>
#include <mir/timer.h>

class C_outer_line:public C_line{
	private:
		void connect();
		void disconnect();
		C_timer * timer;
		bool unpowered;
	public:
		
		C_outer_line(int id);
		~C_outer_line();

		int execute();
		bool active(){return false;}
		
	protected:
		void stage__free();
		void stage__incoming();
		void stage__ready();
		void stage__disconnect();
		void stage__digits();
		void stage__retranslation();
		void stage__connect();
		void stage__wait();

		void signal__line_dialed();
		void signal__line_added();
		void signal__line_deleted();
		void signal__holded_by();
		void signal__unholded_by();
		void signal__holded_freed();
};

extern long line__outer_required_disconnect_timeout;
extern unsigned long line__outer_incoming_ring_timeout;
extern unsigned long line__outer_requried_ring_time;

#endif
