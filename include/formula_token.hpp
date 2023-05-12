#pragma once

#include <string>

class FormulaToken {
   public:
    virtual ~FormulaToken()              = default;
    virtual std::string toString() const = 0;

    enum class TokenType { OPERATION, PARENTHESIS, NUMERIC, CELL_COORD };
    virtual TokenType getTokenType() const = 0;
};