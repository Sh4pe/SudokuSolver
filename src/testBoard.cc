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
				CoordType expected[9];
				for (int i=0; i<3; i++) {
					for (int j=0; j<3; j++) {
						expected[i*3+j].x = uX + j;
						expected[i*3+j].y = uY + i;
					}
				}
				BlockIterator endIt = b.blockEnd(uX, uY);
				int index = 0;
				for (BlockIterator it = b.blockIterator(uX, uY); it != endIt; ++it) {
					CoordType c = it.getCoord();
					ASSERT_EQ(expected[index].x, c.x);
					ASSERT_EQ(expected[index].y, c.y);
					index++;
				}
			}
		}
	}
}

TEST(RowIteratorTest, YieldsCorrectCellValues) {
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
		for (int row=0; row<9; row++) {
			char expected[10];
			for (int i=0; i<9; i++) {
				expected[i] = board[9*row + i];
			}
			expected[9] = '\0';
			//cout << expected << " (expected)" << endl;
			RowIterator endIt = b.rowEnd(0, row);
			char got[10];
			int index = 0;
			for (RowIterator it = b.rowIterator(0, row); it != endIt; ++it) {
				if ((*it).hasValue()) 
					got[index++] = cellValueToChar((*it).value());
				else
					got[index++] = '.';
			}
			got[9] = '\0';
			//cout << got << " (got)" << endl;
			ASSERT_STREQ(expected, got);
		}
	}
}

TEST(RowIteratorTest, YieldsCorrectCoords) {
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
		for (int row=0; row<9; row++) {
			RowIterator endIt = b.rowEnd(0, row);
			int index = 0;
			for (RowIterator it = b.rowIterator(0,row); it != endIt; ++it) {
				CoordType c = it.getCoord();
				ASSERT_EQ(index, c.x);
				ASSERT_EQ(row, c.y);
				index++;
			}
		}
	}
}

TEST(ColumnIteratorTest, YieldsCorrectCellValues) {
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
		for (int column=0; column<9; column++) {
			char expected[10];
			for (int i=0; i<9; i++) {
				expected[i] = board[9*i + column];
			}
			expected[9] = '\0';
			//cout << expected << " (expected)" << endl;
			ColumnIterator endIt = b.columnEnd(column, 0);
			char got[10];
			int index = 0;
			for (ColumnIterator it = b.columnIterator(column, 0); it != endIt; ++it) {
				if ((*it).hasValue())  
					got[index++] = cellValueToChar((*it).value());
				else
					got[index++] = '.';
			}
			got[9] = '\0';
			//cout << got << " (got)" << endl;
			ASSERT_STREQ(expected, got);
		}
	}
}

TEST(ColumnIteratorTest, YieldsCorrectCoords) {
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
		for (int column=0; column<9; column++) {
			ColumnIterator endIt = b.columnEnd(column, 0);
			int index = 0;
			for (ColumnIterator it = b.columnIterator(column, 0); it != endIt; ++it) {
				CoordType c = it.getCoord();
				ASSERT_EQ(column, c.x);
				ASSERT_EQ(index, c.y);
				index++;
			}
		}
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
