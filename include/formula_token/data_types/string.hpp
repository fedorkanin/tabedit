#pragma once

#include <string>

#include "../formula_token.hpp"

class String : public FormulaToken {
   public:
    String(std::string value) : value_(value) {}

    std::string getValue() const { return value_; }

    std::string toString() const override { return value_; }
    TokenType   getTokenType() const override { return TokenType::STRING; }

   private:
    std::string value_;
};