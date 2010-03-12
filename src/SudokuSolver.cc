#include "SudokuSolver.hh"
#include <assert.h>
#include <cstring>
#include <iostream>

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
	// construct the iterators
	for (int uX=0; uX < 3; uX++) {
		for (int uY=0; uY < 3; uY++) {
			int index = uY*3 + uX;
			blockIterators_[index] = BlockIterator(*this, uX*3, uY*3);
			blockEnds_[index] = BlockIterator();
			blockEnds_[index].setEnd(*this, uX*3, uY*3);
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

void Board::setCell(int i, int j, const CellValue& v) {
	cell_[i][j].set(v);
};

Cell& Board::getCell(int i, int j) {
	return cell_[i][j];
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
BlockIterator::BlockIterator(Board& b, int upperX, int upperY) 
	: pos_(0),
	  board_(&b) {
	// fill coords_
	for (int row=0; row<3; row++) {
		for (int col=0; col<3; col++) {
			coords_[3*row+col].x = upperX + row;
			coords_[3*row+col].y = upperY + col;
		}
	}
	blockId_ = upperY + upperX/3;
};

Cell& BlockIterator::operator*() const {
	return board_->getCell(coords_[pos_].x, coords_[pos_].y);
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
		(this->pos_ == oth.pos_) );
};

CoordType BlockIterator::getCoord() const {
	return coords_[pos_];
};

void BlockIterator::setEnd(Board& b, int upperX, int upperY) {
	board_ = &b;
	blockId_ = upperY + upperX/3;
	pos_ = 10;
};
 
/* 
 * Implementation of helper functions
 */
//void reduceCandidateSetsInSlice(
void reduceCandidateSets(Board& b, bool recursive) {
	// find cell with value
	for (int row=0; row < 9; row++) {
		for (int col=0; col<9; col++) {
			return;
		}
	}
};

} // namespace SudokuSolver

