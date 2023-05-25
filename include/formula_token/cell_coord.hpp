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

    /// @brief Checks if a string is a valid cell coordinate
    static bool  isValidCoord(std::string raw_coord);

    TokenType    getTokenType() const override { return TokenType::CELL_COORD; }
    unsigned int getRow() const { return row_; }
    unsigned int getCol() const { return col_; }
    /// @brief Returns the column name of a column number: 0 -> A, 1 -> B, ...
    static std::string   getColName(unsigned int col);

    bool                 operator==(const CellCoord& other) const;
    bool                 operator<(const CellCoord& other) const;

    std::string          toString() const override;
    friend std::ostream& operator<<(std::ostream& os, const CellCoord& coord);
    friend void          to_json(nlohmann::json& j, const CellCoord& p);

   private:
    unsigned int               row_;
    unsigned int               col_;

    /// @brief Parses a coord in format A1, AA1 into a pair of ints
    static std::pair<int, int> parseRawCoord(std::string raw_coord);
};
