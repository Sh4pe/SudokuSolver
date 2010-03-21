/* Copyright (c) 2010 David Nies
 * http://www.twitter.com/Sh4pe
 *  
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *   
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *    
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>
#include <stdlib.h>

/*
 * This is just a small demo of how to use SudokuSolver. The code in
 * SudokuSolver.cc and SudokuSolver.hh is designed and testet much better....
 */

using namespace std;
using namespace SudokuSolver;

void coutUsage() {
	const char* demoboard = "85...24..72......9..4.........1.7..23.5...9...4...........8..7..17..........36.4.";
	cout << "usage: " << endl;
	cout << "sudokusolver [BOARDSTRING] [MAX_BOARDS]" << endl << endl;
	cout << "       Here, BOARDSTRING should be a string of length 81, for example " << endl;
	cout << "       \"" << demoboard << "\"" << endl;
	cout << "       This will set up the board:" << endl;
	Board b;
	b.fromString(demoboard);
	coutBoard(b, "       ");
	cout << "       MAX_BOARDS is the number of solutions after which the solving algorithm stops." << endl;
	cout << endl;
}

int main (int argc, char** argv) {
	if (argc == 3) {
		if (strlen(argv[1]) == 81) {
			long maxSolutionsl = strtol(argv[2], NULL, 10);
			// very bad coding practice...
			unsigned int maxSolutions = static_cast<unsigned int>(maxSolutionsl);
			Board b;
			try {
				b.fromString(argv[1]);
				cout << "Trying to find maximum " << maxSolutions << " solutions for this board: "
				     << endl;
				coutBoard(b);
				list<Board> l;
				try {
					solveBoard(b, l, maxSolutions);
					typedef list<Board>::iterator IT;
					IT endIt = l.end();
					int counter = 1;
					for (IT it = l.begin(); it != endIt; ++it) {
						cout << "Solution " << counter++ << ":"  << endl;
						coutBoard(*it);
						cout << endl;
					}
				}
				catch (InvalidBoardException& e) {
					cout << "This board is invalid!" << endl;
				}

			}
			catch (BoardParseException& e) {
				cout << "Your given BOARDSTRING " << endl; 
				cout << argv[1] << endl;
				cout << "is invalid, check your syntax" << endl;
			}
		}
		else {
			coutUsage();
		}
	}
	else {
		coutUsage();
	}

	return 0;
}
