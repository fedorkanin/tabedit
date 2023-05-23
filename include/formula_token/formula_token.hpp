#pragma once

#include <iostream>
#include <string>

#include "../../libs/json.hpp"

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

    virtual std::string toString() const   = 0;
    using json                             = nlohmann::json;
    virtual json      toJSON() const       = 0;
    virtual TokenType getTokenType() const = 0;
};