#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

class A {
public:
	A() { cout << "Constructor of A." << endl; };
};

class B {
public:
	B() { cout << "Constructor of B." << endl; };
private:
	static A a_;
};
A B::a_ = A();

int main (int argc, char** argv) {
	/*int test[10][10];
	for (int i=0; i<10; i++) {
		test[2][i] = 124;
	}
	for (int i=0; i<10; i++) {
		test[3][i] = 1;
	}
	int (&arrRef)[10] = test[3];
	for (int i=0; i<10; i++) {
		cout << arrRef[i] << " ";
	}
	cout << endl;*/
	B b1 = B();
	B b2 = B();
	return 0;
}
