#include <iostream>
#include <mir/aux/array_.h>


int main(){
	C_array_<int> ar;
	int * i, *j, *p, *q;

	i = new int;
	*i = 1;
	j = new int;
	*j = 2;
	cout << "Creating array..." << endl;
	ar = 2;
	cout << "Adding second element..." << endl;
	ar[1] = i;
	cout << "Adding first element..." << endl;
	ar[0] = j;
	cout << "Extracting first element..." << endl;
	p = ar[0];
	cout << "Extracting second element..." << endl;
	q = ar[1];
	cout << "Len = " << (int) ar << endl;
	cout << "[0] : " << *p << endl;
	cout << "[1] : " << *q << endl;

	return 0;
}
