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
#ifndef SUDOKU_SOLVER_HH
#define SUDOKU_SOLVER_HH 

#include <exception>
#include <iterator>
#include <list>

namespace SudokuSolver {
	
	
/* 
 * Types
 */
enum CellValue { UNDEFINED = -1, EMPTY = 0, ONE = 1, TWO = 2, THREE = 3,
				 FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9};

typedef struct {
	int x;
	int y;
} CoordType;

/*
 * Exceptions
 */
class SudokuSolverException: public std::exception {};
class BoardParseException: public SudokuSolverException {};
class InvalidBoardException: public SudokuSolverException {};

// forward declare
class CandidateSet;
/*
 * class CandidateIterator
 * used to iterate over all candidates in one cell
 */
class CandidateIterator {
public:
	// standard iterator typedefs
	typedef std::forward_iterator_tag iterator_category;
	typedef CellValue value_type;
	typedef size_t difference_type;
	typedef CellValue* pointer;
	typedef CellValue& reference;

	CandidateIterator() : pos_(-1), len_(-1), cSet_(0) {};
	CandidateIterator(CandidateSet& c);

	// used for getting the value 
	CellValue& operator*() const;
	// prefix ++
	CandidateIterator& operator++(); 
	// comparison operators needed in for loops
	bool operator==(CandidateIterator& other);
	bool operator!=(CandidateIterator& other) { return !this->operator==(other); }

private:
	friend class CandidateSet;

	int indices_[9];
	int pos_;
	int len_;
	CandidateSet* cSet_;

	// used to create an endpoint...
	void setEnd(CandidateSet& c);
};


/*
 * class CandidateSet
 */
class CandidateSet {
public:
	// standard constructor
	CandidateSet();
	// copy constructor
	CandidateSet(const CandidateSet& s);
	// v in set?
	bool contains(CellValue& v) const;
	// remove v from set
	void remove(CellValue& v);
	// number of elements in set
	int length() const;
	// length() == 0?
	inline bool isEmpty() const;
	// return first value that is neither UNDEFINED nor EMPTY
	CellValue firstValue() const;
	// sets the candidate set to one element v
	void setOneElement(const CellValue& v);
	void operator=(const CandidateSet& c);

	// returns iterator to iterate over all candidates
	CandidateIterator begin();
	// endpoint for the iteration
	CandidateIterator end();
private:
	friend class CandidateIterator;

	CellValue values_[9];
};

/*
 * Class Cell
 */
class Cell {
public:
	Cell() {};
	// returns the value of the cell, UNDEFINED or EMPTY if there is no value yet
	CellValue value() const;
	// sets the value of the cell to v
	void set(const CellValue& v);
	// returns the underlying CandidateSet 
	CandidateSet& candidates();
	void operator=(const Cell& c);
	// true if value() is neither UNDEFINED nor EMPTY
	bool hasValue();
private:
	CandidateSet candidates_;	
};

/*
 * Slice iterators
 */
// Abstract (virtual) base class
class SliceIterator {
public:
	// standard iterator typedefs
	typedef std::forward_iterator_tag iterator_category;
	typedef Cell value_type;
	typedef size_t difference_type;
	typedef Cell* pointer;
	typedef Cell& reference;

	// used to get the Cell the iterator is refering to
	virtual Cell& operator*() const = 0;
	// prefix ++
	virtual SliceIterator& operator++() = 0; 
	// comparison operators, needed for for loops
	virtual bool operator==(SliceIterator& other) = 0;
	virtual bool operator!=(SliceIterator& other) { return !this->operator==(other); }
	// returns the coordinate of the current cell
	virtual CoordType getCoord() const = 0;
};

// forward declare
class Board;
/*
 * BlockIterator (iterates over one block, from upper left to lower right)
 */
class BlockIterator: public SliceIterator {
public:
	BlockIterator() : pos_(0), blockId_(-1), board_(0) {};
	BlockIterator(Board& b, int upperX, int upperY);

	Cell& operator*() const;
	BlockIterator& operator++();
	bool operator==(SliceIterator& other);
	CoordType getCoord() const;

private:
	friend class Board;
	
	int pos_;
	int blockId_; // number of the block
	static bool coordsInitialized_;
	static CoordType coords_[9][9];
	Board* board_;

	// defines this iterator to be the end of the block (upperX, upperY).
	// thought to be called by friend board after the empty constructor
	// is called
	void setEnd(Board& b, int upperX, int upperY);

	// this is used to initialize coords_. Nice: does not need to be
	// run in setEnd
	static void initializeCoords();
};

/*
 * RowIterator (iterates over a row, from left to right)
 */
class RowIterator: public SliceIterator {
public:
	RowIterator() : pos_(0), rowId_(-1), board_(0) {};
	RowIterator(Board& b, int x, int y);
	
	Cell& operator*() const;
	RowIterator& operator++();
	bool operator==(SliceIterator& other);
	CoordType getCoord() const;
private:
	friend class Board;

	int pos_;
	int rowId_;
	static bool coordsInitialized_;
	static CoordType coords_[9][9];
	Board* board_;

	// see BlockIterator.setEnd
	void setEnd(Board& b, int x, int y);
	// see BlockIterator.initializeCoords
	static void initializeCoords();
};

/*
 * ColumnIterator (iterates over a column, from top to bottom)
 */
class ColumnIterator: public SliceIterator {
public:
	ColumnIterator() : pos_(0), columnId_(-1), board_(0) {};
	ColumnIterator(Board& b, int x, int y);
	
	Cell& operator*() const;
	ColumnIterator& operator++();
	bool operator==(SliceIterator& other);
	CoordType getCoord() const;
private:
	friend class Board;

	int pos_;
	int columnId_;
	static bool coordsInitialized_;
	static CoordType coords_[9][9];
	Board* board_;

	// see BlockIterator.setEnd
	void setEnd(Board& b, int x, int y);
	// see BlockIterator.initializeCoords
	static void initializeCoords();
};

/* CompoundIterator (iterates exactly once over all cells which are in the same block,
 * the same row or the same column as one given cell. There is no guarantee for
 * the order of the iteration)
 */
class CompoundIterator: public SliceIterator {
public:
	CompoundIterator() : pos_(0), cellId_(-1), board_(0) {};
	CompoundIterator(Board& b, int x, int y);
	
	Cell& operator*() const;
	CompoundIterator& operator++();
	bool operator==(SliceIterator& other);
	CoordType getCoord() const;
private:
	friend class Board;

	int pos_;
	int cellId_;
	static bool coordsInitialized_;
	static CoordType coords_[81][21];
	Board* board_;

	// see BlockIterator.setEnd
	void setEnd(Board& b, int x, int y);
	// see BlockIterator.initializeCoords
	static void initializeCoords();
};

/*
 * class Board
 */
class Board {
public:
	Board();
	// copy constructor
	Board(const Board& b);
	void operator=(const Board& b);
	/*
	 * Writes the board into the string buf, assumes that strlen(buf) >= 81.
	 * The board is written rowwise, from top to bottom, and each row is
	 * written from left to right
	 */
	void serialize(char* buf) const;
	// sets the Cell at (row, col) to v
	void setCell(int row, int col, const CellValue& v);
	// returns the value of the Cell at (row, col)
	Cell& getCell(int row, int col);
	// reads a board from str (same format as serialize). Assumes strlen(str) == 81
	void fromString(const char* str);
	// does every cell have a value yet?
	bool isFull();

	/*
	 * These methods are used to get the iterators for the cell (y,x) and the endpoints
	 * of the iteration ( ...End(..) )
	 */
	const BlockIterator& blockIterator(int x, int y);
	const BlockIterator& blockEnd(int x, int y);
	const RowIterator& rowIterator(int x, int y);
	const RowIterator& rowEnd(int x, int y);
	const ColumnIterator& columnIterator(int x, int y);
	const ColumnIterator& columnEnd(int x, int y);
	const CompoundIterator& compoundIterator(int x, int y);
	const CompoundIterator& compoundEnd(int x, int y);
private:
	void initIterators();

	Cell cell_[9][9];

	BlockIterator blockIterators_[9];
	BlockIterator blockEnds_[9];
	RowIterator rowIterators_[9];
	RowIterator rowEnds_[9];
	ColumnIterator columnIterators_[9];
	ColumnIterator columnEnds_[9];
	CompoundIterator compoundIterators_[81];
	CompoundIterator compoundEnds_[81];
};

/*
 * Helper functions
 */
const CellValue charToCellValue(const char c);
const char cellValueToChar(const CellValue& c);
// prints board to std::cout, appends prefix to the left of each line
void coutBoard(Board& b, const char* prefix = "");

/*
 * Solving functions
 */

/*
 * Uses the already set fields to reduce the candidate sets of each slice
 * (i.e. colum, row or block) the given field is in. If recursive==true,
 * this method recursively calls itself whenever it reduces a candidate
 * set to one element, i.e. "finds out" a new value
 */
void reduceCandidateSets(Board& b, bool recursive=true);

/* 
 * Solves the board b. Inserts every solution into solutions, stops if it has
 * found maxSolutions solutions. If you let maxSolutions = n and get a list
 * solutions with m < n entries, you can be sure that these are all solutions.
 * If the board is not solvable, this method throws a InvalidBoardException.
 */
void solveBoard(const Board& b, std::list<Board>& solutions, unsigned int maxSolutions = 1);

/*
 * Checks wheather the board b is valid
 */
bool boardValid(Board& b);

}; // namespace SudokuSolver

#endif

