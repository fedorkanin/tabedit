#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

#include "formula_token.hpp"

class CellCoord : public FormulaToken {
   public:
    CellCoord() = default;
    CellCoord(std::string raw_coord);
    CellCoord(unsigned int row, unsigned int col) : row_(row), col_(col) {}
    static bool  isValidCoord(std::string raw_coord);
    std::string  toString() const override;
    TokenType    getTokenType() const override { return TokenType::CELL_COORD; }
    unsigned int getRow() const { return row_; }
    unsigned int getCol() const { return col_; }
    static std::string getColName(unsigned int col);
    bool               operator==(const CellCoord& other) const {
        return row_ == other.row_ && col_ == other.col_;
    }
    bool operator<(const CellCoord& other) const {
        return row_ < other.row_ || (row_ == other.row_ && col_ < other.col_);
    }
    friend std::ostream& operator<<(std::ostream& os, const CellCoord& coord) {
        os << coord.toString();
        return os;
    }

    friend void to_json(nlohmann::json& j, const CellCoord& p) {
        j = nlohmann::json(p.toString());
    }

   private:
    unsigned int row_;
    unsigned int col_;

    static std::pair<int, int> parseRawCoord(std::string raw_coord);
};
