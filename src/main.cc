#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
#include <cstring>

using namespace std;
using namespace SudokuSolver;

int main (int argc, char** argv) {
	const char* t1 = "hallo bla";
	const char* t2 = "hallo xall";

	for (int i=0; i<10; i++) {
		cout << i << " " << strncmp(t1, t2, i) << endl;
	}
}
