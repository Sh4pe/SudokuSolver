#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "SudokuSolver.hh"
using namespace std;
using namespace SudokuSolver;

TEST(BoardTest, SerializeDeserializeWorks) {
	ifstream validBoards("data/validBoards");
	if (!validBoards) {
		cout << "Could not find data/validBoards!" << endl;
		ASSERT_TRUE(false);
	}	
	char buffer[2048];
	while (validBoards.getline(buffer, 2048)) {
		Board b;
		b.fromString(buffer);
		char buf[90];
		b.serialize(buf);
		// cout << buffer << " (buffer)" << endl << buf << " (buf)" << endl;
		ASSERT_TRUE(strncmp(buffer, buf, 81) == 0);
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
