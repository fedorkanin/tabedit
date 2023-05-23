#pragma once

#include <string>

#include "../formula_token.hpp"

class NoType : public FormulaToken {
   public:
    std::string toString() const override { return "#REF!"; }
    TokenType   getTokenType() const override { return TokenType::NOTYPE; }
    json        toJSON() const override;
};