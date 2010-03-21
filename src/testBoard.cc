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
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>
#include "SudokuSolver.hh"

/*
 * TODO:
 *	- refactor test code!
 */

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

TEST(CompoundIteratorTest, TraversesCorrectCells) {
	for (int x=0; x<9; x++) {
		for (int y=0; y<9; y++) {
			char expected[82];
			strncpy(expected, 
			".................................................................................", 82);
			// change row
			for (int i=0; i<9; i++) {
				expected[9*y + i] = '1';
			}
			// change column
			for (int i=0; i<9; i++) {
				expected[9*i + x] = '1';
			}
			// change block
			int upperX = (x/3)*3;
			int upperY = (y/3)*3;
			for (int a=0; a<3; a++) {
				for (int b=0; b<3; b++) {
					expected[9*(upperY+b) + upperX + a] = '1';
				}
			}
			//cout << expected << " (expected)" << endl;
			// now use the iterator
			Board b;
			CompoundIterator endIt = b.compoundEnd(x,y);
			for (CompoundIterator it = b.compoundIterator(x,y); it != endIt; ++it) {
				(*it).set(ONE);
			}
			char got[82];
			b.serialize(got);
			//cout << got << " (got)" << endl;
			ASSERT_STREQ(expected, got);
		}
	}
}

TEST(CandidateIteratorTest, IteratesOverCorrectNumberOfCells) {
	ifstream validBoards("data/validBoards");
	if (!validBoards) {
		cout << "Could not find data/validBoards!" << endl;
		ASSERT_TRUE(false);
	}	
	char buffer[2048];
	while (validBoards.getline(buffer, 2048)) {
		char board[82];
		strncpy(board, buffer, 81);
		Board b;
		b.fromString(board);
		//coutBoard(b);
		//cout << b.getCell(0,0).candidates().length() << endl;
		for (int x=0; x<9; x++) {
			for (int y=0; y<9; y++) {
				CandidateSet& cs = b.getCell(y,x).candidates();
				int len = cs.length();
				CandidateIterator endIt = cs.end();
				int l = 0;
				for (CandidateIterator it = cs.begin(); it != endIt; ++it) {
					l++;
					// *it should neither be UNDEFINED nor EMPTY
					ASSERT_NE(*it, UNDEFINED);
					ASSERT_NE(*it, EMPTY);
				}
				ASSERT_EQ(len, l);
			}
		}
	}
}

TEST(ReduceCandidateTest, ValidBoardsAreValid) {
	ifstream validBoards("data/validBoards");
	if (!validBoards) {
		cout << "Could not find data/validBoards!" << endl;
		ASSERT_TRUE(false);
	}	
	char buffer[2048];
	while (validBoards.getline(buffer, 2048)) {
		char board[82];
		strncpy(board, buffer, 81);
		Board b;
		b.fromString(board);
		//coutBoard(b);
		try {
			reduceCandidateSets(b, true);
		}
		catch (InvalidBoardException) {
			ASSERT_TRUE(false);
		}
	}	
}

// Let f be reduceCandidateSets. This tests wheather f^2 = f
TEST(ReduceCandidateTest, ReduceCandidateSetIdempotent) {
	ifstream validBoards("data/validBoards");
	if (!validBoards) {
		cout << "Could not find data/validBoards!" << endl;
		ASSERT_TRUE(false);
	}	
	char buffer[2048];
	while (validBoards.getline(buffer, 2048)) {
		char board[82];
		strncpy(board, buffer, 81);
		Board b;
		b.fromString(board);
		//coutBoard(b);
		reduceCandidateSets(b, true);
		char buf1[82];
		b.serialize(buf1);
		reduceCandidateSets(b, true);
		char buf2[82];
		b.serialize(buf2);
		ASSERT_STREQ(buf1, buf2);
	}
}

// Tests wheather the solutions found by solve...() are all different and valid
TEST(SolveBoardTest, AllSolutionsDifferentAndValid) {
	ifstream validBoards("data/validBoards");
	if (!validBoards) {
		cout << "Could not find data/validBoards!" << endl;
		ASSERT_TRUE(false);
	}	
	char buffer[2048];
	while (validBoards.getline(buffer, 2048)) {
		char board[82];
		strncpy(board, buffer, 81);
		Board b;
		b.fromString(board);
		list<Board> l;
		solveBoard(b, l, 20);
		typedef list<Board>::iterator LIT;
		LIT endIt = l.end();
		vector<string> v;
		for (LIT it = l.begin(); it != endIt; ++it) {
			ASSERT_TRUE(boardValid(*it));
			char str[82];
			(*it).serialize(str);
			string s = str;
			v.push_back(s);
			for (int i=0; i<v.size(); i++) {
				for (int j=i+1; j<v.size(); j++) {
					ASSERT_NE(v[i], v[j]);
				}
			}
		}
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
