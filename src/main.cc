#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

int main (int argc, char** argv) {
	Board b;
	CompoundIterator endIt = b.compoundEnd(4,3);
	int index = 0;
	for (CompoundIterator it = b.compoundIterator(4,3); it != endIt; ++it) {
		(*it).set(ONE);
		//cout << index++ << endl;
	}
	coutBoard(b);
	return 0;
}
