#ifndef __MIR__ARRAY_
#define __MIR__ARRAY_

//
//	Динамический массив указателей. Шаблон.
//

#include <stdlib.h>		//	NULL

template <class T>
class C_array_{
	private:
		T ** array;		//	массив указателей
		unsigned int len;	//	длина массива
	public:
		C_array_();
		~C_array_();
		T * & operator[] (unsigned int index);		//	доступ к элементу
		void operator= (unsigned int length);		// 	установить длину
		operator int();					//	вернуть длину
};

template <class T>
C_array_<T>::C_array_():array(NULL), len(0){}

template <class T>
C_array_<T>::~C_array_(){
	if (array) delete array;
}

template <class T>
C_array_<T>::operator int(){
	return len;
}

template <class T>
void C_array_<T>::operator= (unsigned int length){
	T** p = array;
	array = new  T* [length];
	len = length;
	if (p) delete [] p;
}

template <class T>
T * & C_array_<T>::operator[] (unsigned int index){
	if (!len) {
		*this = index + 1;
		return this->operator[](index);
	};
	return array[index % len];
}

#endif
