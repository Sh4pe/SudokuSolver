#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

int main (int argc, char** argv) {
	ifstream validBoards("data/validBoards");
	if (!validBoards) {
		cout << "Could not find data/validBoards!" << endl;
		return 1;
	}	
	char buffer[2048];
	int i = 0;
	while (validBoards.getline(buffer, 2048)) {
		char board[82];
		strncpy(board, buffer, 81);
		Board b;
		b.fromString(board);
		cout << "before: " << endl;
		coutBoard(b);
		list<Board> l;
		solveBoard(b, l, 5);
		cout << "after: (" << l.size() << " solutions found " << endl;
		typedef list<Board>::iterator IT;
		IT endIt = l.end();
		for (IT it = l.begin(); it != endIt; ++it) {
			coutBoard(*it);
			cout << endl;
		}
	}
	return 0;
}
