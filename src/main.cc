#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

int main (int argc, char** argv) {

	Board b;
	b.fromString("85...24..72......9..4.........1.7..23.5...9...4...........8..7..17..........36.4.");
	coutBoard(b);
	list<Board> l;
	solveBoard(b, l, 10);
	cout << "solution(s): " << endl << endl;
	list<Board>::iterator endIt = l.end();
	for (list<Board>::iterator it = l.begin(); it != endIt; ++it) {
		cout << endl;
		coutBoard(*it);
	}

	return 0;
}
