#ifndef   __MIR__RING_
#define   __MIR__RING_

//
//	Кольцо указателей на
//	объекты. Шаблон
//
//	Объем памяти = 3N
//

#include <stdlib.h>		//	NULL

template <class T>
class C_ring_{
	private:
		struct ring_item{
			ring_item * next;
			ring_item * prev;
			T * data;
		};
		
		int count;			//	счетчик кол-ва элементов
		struct ring_item * ring;	//	указатель на первый элемент кольца
		int kill();			//	удалить первый элемент (не содержимое элемента) кольца

	public:
		C_ring_();
		virtual ~C_ring_();
		virtual int add(T* n);		//	добавить элемент в конец кольца
		virtual int push(T* n);		//	добавить элемент в начало кольца
		virtual T* get(int n = 0);	//	вернуть n-ый элемент кольца
		virtual T* pop(int n = 0);	//	вернуть и удалить из кольца n-ый элемент кольца
		virtual void roll(int n = 1);	//	прокрутить кольцо: + вперед /  - назад
		virtual int quantity();		//	вернуть колчество элементов в кольце
};

template <class T>
C_ring_<T>::C_ring_(){
	ring = NULL;
	count = 0;
}

template <class T>
int C_ring_<T>::add(T * new_item){
	//	добавить элемент в кольцо	
	struct ring_item * new_i;

	if (!ring){	//	если пустое кольцо, добавляем в голову
		new_i = new struct ring_item;
		if (new_i == NULL) return -1;
		new_i->data = new_item;
		new_i->next = new_i;
		new_i->prev = new_i;
		ring = new_i;
		count++;
	}else{			//	иначе, добавление в конец	
		struct ring_item * first = ring;
		struct ring_item * last = ring->prev;

		new_i = new struct ring_item;
		if (!new_i) return -1;
		new_i->data = new_item;
		new_i->next = first;
		new_i->prev = last;
		last->next = new_i;
		first->prev = new_i;
		count++;
	}
	return 0;
}

template <class T>
int C_ring_<T>::push(T * new_item){
	if (add(new_item)) return -1;
	roll(-1);
	return 0;
}

template <class T>
int C_ring_<T>::kill(){
	//	удаление из головы кольца
	
	if (!ring) return -1;	//	кольцо уже пустое
	if (ring->next == ring){
		delete ring;
		ring = NULL;
	}else{
		struct ring_item * next = ring->next;
		struct ring_item * prev = ring->prev;

		next->prev = prev;
		prev->next = next;

		delete ring;
		ring = next;
	}
	count--;
	return 0;
}

template <class T>
T * C_ring_<T>::pop(int n){
	//	извлечение элемента
	
	T * d = get(n);				//	получил элемент
	struct ring_item * first = ring;	//	сохранил голову

	if (n) roll(n);				//	прокрутил, если n!=0
	kill();					//	удалил из головы
	if (n) ring = first;			//	востановил голову, если n!=0
	return d;				//	вернул элемент
}

template <class T>
T * C_ring_<T>::get(int n){
	//	вернуть элемент кольца
	
	struct ring_item * item = ring;

	if (!item) return NULL;
	while (n < 0){
		item = item->prev;
		n++;
	}
	while (n>0){
		item = item->next;
		n--;
	}
	return item->data;
}

template <class T>
void C_ring_<T>::roll(int n){
	//	прокрутить, т.е. изменить позицию головы
	
	if (ring == NULL) return;
	while (n<0){
		ring = ring->prev;
		n++;
	}
	while (n>0){
		ring = ring->next;
		n--;
	}
}

template <class T>
C_ring_<T>::~C_ring_(){
//	удалить все элементы кольца

	while (kill() == 0);
}

template <class T>
int C_ring_<T>::quantity(){
	return count;
}


#endif
