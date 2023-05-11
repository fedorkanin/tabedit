#pragma once

#include <string>

#include "formula_token.hpp"

class CellCoord : public FormulaToken {
   public:
    CellCoord(std::string raw_coord);
    static bool isValidCoord(std::string raw_coord);
    std::string toString() const override;

   private:
    unsigned int row_;
    unsigned int col_;

    static std::pair<int, int> parseRawCoord(std::string raw_coord);
};