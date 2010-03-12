#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

int main (int argc, char** argv) {
	
	Board b;
	b.fromString("651.72..98...5.76.7.4..9..2...7.....49..68...5.6.1..7..7....5...4...7..83.5....2.");
	BlockIterator endIt = b.blockEnd(0,0);
	int i = 0;
	for (BlockIterator it = b.blockIterator(0,0); it != endIt; ++it) {
		//cout << "in loop! " << (i++) << endl;
		//cout << (it != endIt) << endl;
		if ((*it).hasValue())
			cout << cellValueToChar((*it).value());
		else
			cout << '.';
	}
	cout << endl;
	return 0;
}
