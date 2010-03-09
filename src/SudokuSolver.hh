#ifndef SUDOKU_SOLVER_HH
#define SUDOKU_SOLVER_HH 

#include <exception>

namespace SudokuSolver {
	
	
// Types
enum CellValue { UNDEFINED = -1, EMPTY = 0, ONE = 1, TWO = 2, THREE = 3,
				 FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9};

// Exceptions
class SudokuSolverException: public std::exception {};
class BoardParseException: public SudokuSolverException {};

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
	void remove(CellValue& v);
	int length() const;
	inline bool isEmpty() const;
	// return first value that is neither UNDEFINED nor EMPTY
	CellValue firstValue() const;
	// sets the candidate set to one element v
	void setOneElement(const CellValue& v);
	void operator=(const CandidateSet& c);
private:
	CellValue values_[9];
};

/*
 * Class Cell
 */
class Cell {
public:
	Cell() {};
	CellValue value() const;
	void set(const CellValue& v);
	CandidateSet& candidates();
	void operator=(const Cell& c);
private:
	CandidateSet candidates_;	
};

/*
 * class Board
 */
class Board {
public:
	Board() {};
	Board(const Board& b);
	void operator=(const Board& b);
	// expects buf to have length 82 (closing with \0)
	void serialize(char* buf) const;
	void setCell(int i, int j, const CellValue& v);
	// reads a board from str (same format as serialize). Assumes strlen(str) == 81
	void fromString(const char* str);
private:
	Cell cell_[9][9];
};


} // namespace SudokuSolver

#endif

