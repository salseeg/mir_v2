#include <iostream>

#include <mir/aux/ring.h>

/*
*	Test of abstract ring
*
*/


int main(){
	C_ring * ring;
	int a, b, c;
	int i,n;

	a = 1;
	
	b = 2;
	
	c = 3;

	cout << "Creating ring ..." << endl;
	ring = new C_ring;
	
	cout << "Adding a,b,c ..." << endl;
	ring->add(&a);
	ring->add(&b);
	ring->add(&c);
	
	ring->roll(1);
	n = ring->quantity();
	for (i=0;i<n;i++){
		int * p = (int *) ring->pop(); 
		cout <<  *p << endl;
	}
	delete ring;
	return 0;
}
