#ifndef   __MIR__RING_
#define   __MIR__RING_

//
//	������ ���������� ��
//	�������. ������
//
//	����� ������ = 3N
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
		
		int count;			//	������� ���-�� ���������
		struct ring_item * ring;	//	��������� �� ������ ������� ������
		int kill();			//	������� ������ ������� (�� ���������� ��������) ������

	public:
		C_ring_();
		virtual ~C_ring_();
		virtual int add(T* n);		//	�������� ������� � ����� ������
		virtual int push(T* n);		//	�������� ������� � ������ ������
		virtual T* get(int n = 0);	//	������� n-�� ������� ������
		virtual T* pop(int n = 0);	//	������� � ������� �� ������ n-�� ������� ������
		virtual void roll(int n = 1);	//	���������� ������: + ������ /  - �����
		virtual int quantity();		//	������� ��������� ��������� � ������
};

template <class T>
C_ring_<T>::C_ring_(){
	ring = NULL;
	count = 0;
}

template <class T>
int C_ring_<T>::add(T * new_item){
	//	�������� ������� � ������	
	struct ring_item * new_i;

	if (!ring){	//	���� ������ ������, ��������� � ������
		new_i = new struct ring_item;
		if (new_i == NULL) return -1;
		new_i->data = new_item;
		new_i->next = new_i;
		new_i->prev = new_i;
		ring = new_i;
		count++;
	}else{			//	�����, ���������� � �����	
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
	//	�������� �� ������ ������
	
	if (!ring) return -1;	//	������ ��� ������
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
	//	���������� ��������
	
	T * d = get(n);				//	������� �������
	struct ring_item * first = ring;	//	�������� ������

	if (n) roll(n);				//	���������, ���� n!=0
	kill();					//	������ �� ������
	if (n) ring = first;			//	���������� ������, ���� n!=0
	return d;				//	������ �������
}

template <class T>
T * C_ring_<T>::get(int n){
	//	������� ������� ������
	
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
	//	����������, �.�. �������� ������� ������
	
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
//	������� ��� �������� ������

	while (kill() == 0);
}

template <class T>
int C_ring_<T>::quantity(){
	return count;
}


#endif
