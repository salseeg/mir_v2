#include <iostream>

#include "../aux/ring_.h"
/*
*	Test of ring template
*
*/
class sint{
	private:
		int i;
	public:
		sint(int n = 0):i(n){cout << "constructor (" << n << ")" << endl;}
		sint(const sint& n):i(n.i){cout << "copy constructor (" << n.i << ")" << endl;}
		~sint(){cout << "destructor" << endl;}
		sint & operator=(int n){
			i = n;
			cout << "operator=  (" << n << ")" << endl;
			return *this;
		}
		operator int(){
			cout << "operator int" << endl;
			return i;
		}
};

int main(){
	C_ring_<sint> ring;
	sint a, b, c;
	int i,n;

	a = 1;
	
	b = 2;
	
	c = 3;


	cout << "Adding a,b,c ..." << endl;
	ring.add(&a);
	ring.add(&b);
	ring.add(&c);
	
	ring.roll(1);
	n = ring.quantity();
	for (i = 0; i < n; i++){
		sint p = *ring.pop(); 
		cout << p << endl;
	}
	cout << "ring elements quantity " << ring.quantity() << endl;
	return 0;
}
