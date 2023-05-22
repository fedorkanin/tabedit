#pragma once

#include <string>

class FormulaToken {
   public:
    virtual ~FormulaToken() = default;
    enum class TokenType {
        OPERATION,
        PARENTHESIS,
        CELL_COORD,
        STRING,
        INTEGER,
        DOUBLE,
        NOTYPE
    };

    virtual std::string toString() const     = 0;
    virtual TokenType   getTokenType() const = 0;
};