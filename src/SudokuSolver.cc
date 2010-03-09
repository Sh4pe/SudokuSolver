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

/*
 * Implementation Board
 */
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

} // namespace SudokuSolver

