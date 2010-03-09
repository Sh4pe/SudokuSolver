#include <iostream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

int main (int argc, char** argv) {
	const char* b = "123456789123456789123456789123456789123456789123456789123456789123456789123456789";
	Board brd;
	brd.fromString(b);
	char buf[81];
	brd.serialize(buf);
	cout << buf << endl; //unittest
}

