#ifndef __MIR__INNER_LINE
#define __MIR__INNER_LINE

#include "line.h"

class C_inner_line:public C_line{
	public:
		C_inner_line(int id_, int priori = 0);
		~C_inner_line();
		bool active();
	protected:
		void stage__free();
		void stage__ready();
		void stage__digits();
		void stage__wait();
		void stage__connect();
		void stage__disconnect();
		void stage__retranslation();
		void stage__incoming();
			
		void signal__line_dialed();
		void signal__line_deleted();
		void signal__line_added();
		void signal__holded_by();
		void signal__unholded_by();
		void signal__holded_freed();
};

extern unsigned long line__ready_timeout;
extern unsigned long line__required_disconnet_time;
extern unsigned long line__wait_4_connect_timeout;

#endif
