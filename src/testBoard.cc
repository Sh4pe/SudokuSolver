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

TEST(BlockIteratorTest, YieldsCorrectCellValues) {
	ifstream validBoards("data/validBoards");
	if (!validBoards) {
		cout << "Could not find data/validBoards!" << endl;
		ASSERT_TRUE(false);
	}	
	char buffer[2048];
	while (validBoards.getline(buffer, 2048)) {
		Board b;
		char board[82];
		strncpy(board, buffer, 81);
		b.fromString(board);
		//coutBoard(b);
		for (int uY=0; uY<9; uY += 3) {
			for (int uX=0; uX<9; uX += 3) {
				// construct expected string
				char expected[10];
				for (int i=0; i<3; i++) {
					for (int j=0; j<3; j++) {
						expected[i*3+j] = board[9*(uY+i) + 3*(uX/3) + j];
					}
				}
				expected[9] = '\0';
				//cout << expected << " (expected)" << endl;
				// construct string obtained through iterator
				char got[10];
				int index = 0;
				//cout << "(uX, uY): (" << uX << ", " << uY << ")" << endl;
				BlockIterator endIt = b.blockEnd(uX, uY);
				for (BlockIterator it = b.blockIterator(uX, uY); it != endIt; ++it) {
					Cell& c = *it;
					if (c.hasValue())
						got[index++] = cellValueToChar(c.value());
					else
						got[index++] = '.';
				}
				got[9] = '\0';
				//cout << got << " (got) " << endl;
				ASSERT_STREQ(expected, got);
			}
		}
	}
}

TEST(BlockIteratorTest, YieldsCorrectCoords) {
	implementieren
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
