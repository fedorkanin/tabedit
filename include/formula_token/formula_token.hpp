#pragma once

#include <iostream>
#include <string>

#include "../../libs/json.hpp"

/// @brief Abstract class for all formula tokens: operands, operations,
/// parenthesis, coordinates
class FormulaToken {
   public:
    virtual ~FormulaToken() = default;

    /// @brief Enum class for token types. Used for safe runtime type checking
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