#include "SudokuSolver.hh"
#include <assert.h>
#include <cstring>
#include <iostream>

/* TODO:
 * 	- check where you can skip the .hasValue()-Test - .value() works anyway
 */

using namespace std;

namespace SudokuSolver {
	
/*
 * Implementation CandidateSet
 */
CandidateSet::CandidateSet() {
	for (size_t i = 0; i < 9; ++i) {
		values_[i] = CellValue(i);
	}
};

CandidateSet::CandidateSet(const CandidateSet& s) {
	for (size_t i = 0; i < 9; ++i) {
		values_[i] = s.values_[i];
	}
	//std::cout << "CandidateSet copy constructor" << std::endl;
};

bool CandidateSet::contains(CellValue& v) const {
	assert((v != UNDEFINED) && (v != EMPTY));
	for (size_t i = 0; i < 9; ++i) {
		if (values_[i] == v)
			return true;
	}
	return false;
};

void CandidateSet::remove(CellValue& v) {
	assert((v != UNDEFINED) && (v != EMPTY));
	for (size_t i = 0; i < 9; ++i) {
		if (values_[i] == v) {
			values_[i] = UNDEFINED;
			break;
		}
	}
};

int CandidateSet::length() const {
	int len = 0;
	for (size_t i = 0; i < 9; ++i) {
		if ((values_[i] != UNDEFINED) && (values_[i] != EMPTY))
			++len;
	}
	assert(len > 0);
	return len;
};

inline bool CandidateSet::isEmpty() const {
	return (this->length() > 1);
};

CellValue CandidateSet::firstValue() const {
	for (size_t i = 0; i < 8; ++i) {
		if ((values_[i] != UNDEFINED) && (values_[i] != EMPTY)) {
			return values_[i];
		}
	}
	return values_[8];
};

void CandidateSet::setOneElement(const CellValue& v) {
	values_[0] = v;
	for (size_t i = 1; i < 9; ++i) {
		values_[i] = UNDEFINED;
	}
};

void CandidateSet::operator=(const CandidateSet& c) {
	for (size_t i = 0; i < 9; ++i) {
		values_[i] = c.values_[i];
	}
};

/*
 * Implementation Cell
 */
CellValue Cell::value() const {
	assert(candidates_.length() > 0);
	if (candidates_.length() > 1)
		return EMPTY;
	return candidates_.firstValue();
};

void Cell::set(const CellValue& v) {
	candidates_.setOneElement(v);
};

CandidateSet& Cell::candidates() {
	return candidates_;
};

void Cell::operator=(const Cell& c) {
	candidates_ = c.candidates_;
};

bool Cell::hasValue() {
	return (candidates_.length() == 1);
};
/*
 * Implementation Board
 */
Board::Board() {
	// construct BlockIterators
	for (int uX=0; uX < 3; uX++) {
		for (int uY=0; uY < 3; uY++) {
			int index = uY*3 + uX;
			blockIterators_[index] = BlockIterator(*this, uX*3, uY*3);
			blockEnds_[index] = BlockIterator();
			blockEnds_[index].setEnd(*this, uX*3, uY*3);
		}
	}
	// construct RowIterators
	for (int row=0; row<9; row++) {
		rowIterators_[row] = RowIterator(*this, 0, row);
		rowEnds_[row] = RowIterator();
		rowEnds_[row].setEnd(*this, 0, row);
	}
	// construct ColumnIterators
	for (int column=0; column<9; column++) {
		columnIterators_[column] = ColumnIterator(*this, column, 0);
		columnEnds_[column] = ColumnIterator();
		columnEnds_[column].setEnd(*this, column, 0);
	}
	// construct CompoundIterators
	for (int x=0; x<9; x++) {
		for (int y=0; y<9; y++) {
			compoundIterators_[9*y+x] = CompoundIterator(*this, x, y);
			compoundEnds_[9*y+x] = CompoundIterator();
			compoundEnds_[9*y+x].setEnd(*this, x, y);
		}
	}
}

Board::Board(const Board& b) {
	for (size_t i = 0; i < 9; ++i) {
		for (size_t j = 0; j < 9; ++j) {
			cell_[i][j] = b.cell_[i][j];
		}
	}
};

void Board::operator=(const Board& b) {
	for (size_t i = 0; i < 9; ++i) {
		for (size_t j = 0; j < 9; ++j) {
			cell_[i][j] = b.cell_[i][j];
		}
	}
};

void Board::serialize(char* buf) const {
	const char* DIGITS = "0123456789";
	for (size_t row = 0; row < 9; ++row) {
		for (size_t col = 0; col < 9; ++col) {
			CellValue v = cell_[row][col].value();
			char c;
			if (v == EMPTY) {
				c = '.';
			}
			else {
				c = DIGITS[(size_t)v];
			}
			buf[9*row+col] = c;
		}
	}
	buf[81] = '\0';
};

void Board::setCell(int row, int col, const CellValue& v) {
	cell_[row][col].set(v);
};

Cell& Board::getCell(int row, int col) {
	return cell_[row][col];
};

void Board::fromString(const char* str) {
	const char* allowed = ".0123456789";
	for (size_t i = 0; i < 81; ++i) {
		if (strchr(allowed, str[i]) == NULL)
			throw BoardParseException();
		CellValue v = charToCellValue(str[i]);
		if ((v != UNDEFINED) && (v != EMPTY))
			cell_[i/9][i%9].set(v);
	}
};

const BlockIterator& Board::blockIterator(int x, int y) {
	// int multiplication is not associtive, so the following line makes sence
	return blockIterators_[(y/3)*3 + x/3];
};

const BlockIterator& Board::blockEnd(int x, int y) {
	// int multiplication is not associtive, so the following line makes sence
	return blockEnds_[(y/3)*3 + x/3];
};

const RowIterator& Board::rowIterator(int x, int y) {
	return rowIterators_[y];
};

const RowIterator& Board::rowEnd(int x, int y) {
	return rowEnds_[y];
};

const ColumnIterator& Board::columnIterator(int x, int y) {
	return columnIterators_[x];
};

const ColumnIterator& Board::columnEnd(int x, int y) {
	return columnEnds_[x];
};

const CompoundIterator& Board::compoundIterator(int x, int y) {
	return compoundIterators_[9*y+x];
};

const CompoundIterator& Board::compoundEnd(int x, int y) {
	return compoundEnds_[9*y+x];
};

/*
 * Helper function used by Board
 *	assumes that c is in {.,0, .. , 9}
 */
const CellValue charToCellValue(const char c) {
	switch (c) {
		case '.': return UNDEFINED;
		case '1': return ONE;
		case '2': return TWO;
		case '3': return THREE;
		case '4': return FOUR;
		case '5': return FIVE;
		case '6': return SIX;
		case '7': return SEVEN;
		case '8': return EIGHT;
		case '9': return NINE;
	}
	return EMPTY;
}

const char cellValueToChar(const CellValue& c) {
	switch (c) {
		case UNDEFINED	: return '.';
		case ONE	: return '1';
		case TWO	: return '2';
		case THREE	: return '3';
		case FOUR	: return '4';
		case FIVE	: return '5';
		case SIX	: return '6';
		case SEVEN	: return '7';
		case EIGHT	: return '8';
		case NINE	: return '9';
	}
	return '.';
};

/*
 * Implementation BlockIterator
 */
// initialize static members
bool BlockIterator::coordsInitialized_ = false;
CoordType BlockIterator::coords_[9][9] = { 0 };

BlockIterator::BlockIterator(Board& b, int upperX, int upperY) 
	: pos_(0),
	  board_(&b) {
	// fill coords_
	initializeCoords();
	blockId_ = upperY + upperX/3;
};

Cell& BlockIterator::operator*() const {
	return board_->getCell(coords_[blockId_][pos_].y, coords_[blockId_][pos_].x);
};

BlockIterator& BlockIterator::operator++() {
	++pos_;
	return *this;
};

bool BlockIterator::operator==(SliceIterator& other) {
	if (typeid(other) != typeid(BlockIterator))
		return false;
	// so it seems to be a BlockIterator
	BlockIterator& oth = static_cast<BlockIterator&>(other);
	return ((this->blockId_ == oth.blockId_) && 
		(this->pos_ == oth.pos_) &&
		(this->board_ == oth.board_) );
};

CoordType BlockIterator::getCoord() const {
	return coords_[blockId_][pos_];
};

void BlockIterator::setEnd(Board& b, int upperX, int upperY) {
	board_ = &b;
	blockId_ = upperY + upperX/3;
	pos_ = 9;
};

void BlockIterator::initializeCoords() {
	if (BlockIterator::coordsInitialized_)
		return;

	for (int upperX=0; upperX<9; upperX += 3) {
		for (int upperY=0; upperY<9; upperY += 3) {
			for (int row=0; row<3; row++) {
				for (int col=0; col<3; col++) {
					coords_[upperY + upperX/3][3*row+col].x = upperX + col;
					coords_[upperY + upperX/3][3*row+col].y = upperY + row;
				}
			}
		}
	}
	BlockIterator::coordsInitialized_ = true;
};

/*
 * Implementation of RowIterator
 */
// initialization of static members
CoordType RowIterator::coords_[9][9] = { 0 };
bool RowIterator::coordsInitialized_ = false;

RowIterator::RowIterator(Board& b, int x, int y) 
	: pos_(0),
	  board_(&b),
	  rowId_(y) {
	initializeCoords();
};

Cell& RowIterator::operator*() const {
	return board_->getCell(coords_[rowId_][pos_].y, coords_[rowId_][pos_].x);
};

RowIterator& RowIterator::operator++() {
	++pos_;
	return *this;
};
 
bool RowIterator::operator==(SliceIterator& other) {
	if (typeid(other) != typeid(RowIterator))
		return false;
	// so it seems to be a RowIterator
	RowIterator& oth = static_cast<RowIterator&>(other);
	return ((this->rowId_ == oth.rowId_) && 
		(this->pos_ == oth.pos_) &&
		(this->board_ == oth.board_) );

};

CoordType RowIterator::getCoord() const {
	return coords_[rowId_][pos_];
};

void RowIterator::setEnd(Board& b, int x, int y) {
	rowId_ = y;
	pos_ = 9;
	board_ = &b;
};

void RowIterator::initializeCoords() {
	if (coordsInitialized_)
		return;

	for (int i=0; i<9; i++) {
		for (int col=0; col<9; col++) {
			coords_[i][col].x = col;
			coords_[i][col].y = i;
		}
	}
	RowIterator::coordsInitialized_ = true;
};

/*
 * Implementation of ColumnIterator
 */
// initialize static members
CoordType ColumnIterator::coords_[9][9] = { 0 };
bool ColumnIterator::coordsInitialized_ = false;

ColumnIterator::ColumnIterator(Board& b, int x, int y) 
	: pos_(0),
	  board_(&b),
	  columnId_(x) {
	initializeCoords();
};

Cell& ColumnIterator::operator*() const {
	return board_->getCell(coords_[columnId_][pos_].y, coords_[columnId_][pos_].x);
};

ColumnIterator& ColumnIterator::operator++() {
	++pos_;
	return *this;
};
 
bool ColumnIterator::operator==(SliceIterator& other) {
	if (typeid(other) != typeid(ColumnIterator))
		return false;
	// so it seems to be a ColumnIterator
	ColumnIterator& oth = static_cast<ColumnIterator&>(other);
	return ((this->columnId_ == oth.columnId_) && 
		(this->pos_ == oth.pos_) &&
		(this->board_ == oth.board_) );

};

CoordType ColumnIterator::getCoord() const {
	return coords_[columnId_][pos_];
};

void ColumnIterator::setEnd(Board& b, int x, int y) {
	columnId_ = x;
	pos_ = 9;
	board_ = &b;
};

void ColumnIterator::initializeCoords() {
	if (coordsInitialized_)
		return;

	for (int i=0; i<9; i++) {
		for (int row=0; row<9; row++) {
			coords_[i][row].x = i;
			coords_[i][row].y = row;
		}
	}
	ColumnIterator::coordsInitialized_ = true;
};

/*
 * Implementation of CompoundIterator
 */
// initialize static members
CoordType CompoundIterator::coords_[81][21] = { 0 };
bool CompoundIterator::coordsInitialized_ = false;

CompoundIterator::CompoundIterator(Board& b, int x, int y) 
	: pos_(0),
	  board_(&b) {
	cellId_ = 9*y + x;
	initializeCoords();
};

Cell& CompoundIterator::operator*() const {
	return board_->getCell(coords_[cellId_][pos_].y, coords_[cellId_][pos_].x);
};

CompoundIterator& CompoundIterator::operator++() {
	++pos_;
	return *this;
};
 
bool CompoundIterator::operator==(SliceIterator& other) {
	if (typeid(other) != typeid(CompoundIterator))
		return false;
	// so it seems to be a CompoundIterator
	CompoundIterator& oth = static_cast<CompoundIterator&>(other);
	return ((this->cellId_ == oth.cellId_) && 
		(this->pos_ == oth.pos_) &&
		(this->board_ == oth.board_) );

};

CoordType CompoundIterator::getCoord() const {
	return coords_[cellId_][pos_];
};

void CompoundIterator::setEnd(Board& b, int x, int y) {
	cellId_ = 9*y + x;
	pos_ = 21;
	board_ = &b;
};

void CompoundIterator::initializeCoords() {
	if (coordsInitialized_)
		return;

	for (int x=0; x<9; x++) {
		for (int y=0; y<9; y++) {
			int upperX = 3*(x/3);
			int upperY = 3*(y/3);
			// add block coords first
			for (int a=0; a<3; a++) {
				for (int b=0; b<3; b++) {
					coords_[y*9+x][3*a+b].x = upperX + b;
					coords_[y*9+x][3*a+b].y = upperY + a;
				}
			}
			// then the column cells
			if (x < 3) {
				for (int i=0; i<6; i++) {
					coords_[y*9+x][9+i].x = i+3;
					coords_[y*9+x][9+i].y = y;
				}
			}
			else if (x < 6) { // 3 <= x < 6
				for (int i=0; i<3; i++) {
					coords_[y*9+x][9+i].x = i;
					coords_[y*9+x][9+i].y = y;
				}
				for (int i=0; i<3; i++) {
					coords_[y*9+x][12+i].x = 6+i;
					coords_[y*9+x][12+i].y = y;
				}
			}
			else { // 6 <= x
				for (int i=0; i<6; i++) {
					coords_[y*9+x][9+i].x = i;
					coords_[y*9+x][9+i].y = y;
				}
			}
			// and finally row cells
			if (y < 3) {
				for (int i=0; i<6; i++) {
					coords_[y*9+x][15+i].x = x;
					coords_[y*9+x][15+i].y = i+3;
				}
			}
			else if (y < 6) { // 3 <= y < 6
				for (int i=0; i<3; i++) {
					coords_[y*9+x][15+i].x = x;
					coords_[y*9+x][15+i].y = i;
				}
				for (int i=0; i<3; i++) {
					coords_[y*9+x][18+i].x = x;
					coords_[y*9+x][18+i].y = 6+i;
				}
			}
			else { // 6 <= y
				for (int i=0; i<6; i++) {
					coords_[y*9+x][15+i].x = x;
					coords_[y*9+x][15+i].y = i;
				}
			}
		}
	}
	CompoundIterator::coordsInitialized_ = true;
};

/* 
 * Implementation of helper functions
 */
void coutBoard(Board& b) {
	for (int threeRow=0; threeRow<3; threeRow++) {
		cout << " ----------------- " << endl;
		for (int row=0; row<3; row++) {
			cout << "| ";
			for (int j=0; j<3; j++) {
				for (int i=0; i<3; i++) {
					Cell& c = b.getCell(row + 3*threeRow, i+3*j);
					if (c.hasValue())
						cout << cellValueToChar(c.value());
					else
						cout << '.';
				}
				cout << " | ";
			}
			cout << endl;
		}
	}
	cout << " ----------------- " << endl;
};
/* * Implementation of solving functions
 */

/* 
 * Helper for reduceCandidateSets
 * Drops the value of the cell (row, col) from all slices belonging to (row, col).
 * recursive == true: does this recursively
 */
void dropCandidateFromSlices(Board& b, int row, int col, bool recursive = true) {
	CellValue v = b.getCell(row, col).value();
	assert((v != UNDEFINED) && (v != EMPTY));
	BlockIterator endBlockIt = b.blockEnd(col, row);
	for (BlockIterator blockIt = b.blockIterator(col, row); blockIt != endBlockIt; ++blockIt) {
		CoordType c = blockIt.getCoord();
		// leave (row, col) out
		if ((c.x != col) || (c.y != row)) {
			// go on here
		}
	}
}

//void reduceCandidateSetsInSlice
void reduceCandidateSets(Board& b, bool recursive) {
	// find cell with value
	for (int row=0; row < 9; row++) {
		for (int col=0; col<9; col++) {
			return;
		}
	}
};

} // namespace SudokuSolver

