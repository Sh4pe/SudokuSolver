#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

int main (int argc, char** argv) {
	
	Board b;
	b.fromString("651.72..98...5.76.7.4..9..2...7.....49..68...5.6.1..7..7....5...4...7..83.5....2.");
	BlockIterator endIt = b.blockEnd(1,1);
	for (BlockIterator it = b.blockIterator(1,1); it != endIt; ++it) {
		if ((*it).hasValue())
			cout << cellValueToChar((*it).value());
		else
			cout << '.';
	}
	return 0;
}
