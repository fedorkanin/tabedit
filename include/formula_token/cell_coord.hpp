#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>

#include "formula_token.hpp"

class CellCoord : public FormulaToken {
   public:
    CellCoord(std::string raw_coord);
    static bool  isValidCoord(std::string raw_coord);
    std::string  toString() const override;
    TokenType    getTokenType() const override { return TokenType::CELL_COORD; }
    unsigned int getRow() const { return row_; }
    unsigned int getCol() const { return col_; }

   private:
    unsigned int row_;
    unsigned int col_;

    static std::pair<int, int> parseRawCoord(std::string raw_coord);
};