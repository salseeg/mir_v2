#ifndef __MIR__HARD
#define __MIR__HARD
//
//	Взаимодействие с аппаратным уровнем
//
	
// TODO оптимизация изменений ключей

#include "interface/driver.h"

enum inner_key{		// Ключи внутренних линий
	inner_key__kna,	//	сигнала
	inner_key__pv	//	посылка вызова
};
const int inner_key_count = 2;

enum outer_key{		// Ключи внешних линий
	outer_key__kv,	//	шлейфа
	outer_key__sk,	//	шунтирующий
	outer_key__ik	//	импульсный
};
const int outer_key_count = 3;

class C_hard{
	private:
		int driver_descriptor;		//	дескриптор драйвера
		int n_inner;			//	кол-во внутр линий
		int n_outer;			//	кол-во внешн линий
		
		bit_history * inner_states;	//	состояния внутр линий
		bit_history * outer_states;	//	состояния внешн линий
		
		on_off * kna;
		on_off * pv;
		on_off * kv;
		on_off * sk;
		on_off * ik;
		
		bool kna_m;
		bool pv_m;
		bool kv_m;
		bool sk_m;
		bool ik_m;

		unsigned long delta;
		
		void read();			//	считать состояния

	public:
		
		C_hard(char * driver_filename, int n_inner, int n_outer);
		~C_hard();

		void matrix(int cord, int matrix_line, on_off st);
		void sleep_us();
		void set_inner(int inner_line, inner_key key, on_off st);
		void set_outer(int outer_line, outer_key key, on_off st);
		bit_history & get_state_inner(int inner_line);
		bit_history & get_state_outer(int outer_line);

		
		unsigned long get_delta();
};

extern C_hard * Hard;

#endif
