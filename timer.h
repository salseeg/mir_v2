#ifndef __MIR__TIMER
#define __MIR__TIMER

class C_timer{
	private:
		long time;
	public:
		C_timer(long t):time(t){}	//	time in usec

		bool check(){return (time <= 0);}
		void execute(long dt){if (!check()) time -= dt;}	
};

#endif
