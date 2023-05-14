#pragma once

#include <string>

#include "../formula_token.hpp"

class Integer : public FormulaToken {
   public:
    Integer(long long value) : value_(value) {}
    Integer(std::string value) : value_(std::stoll(value)) {}

    long long getValue() const { return value_; }

    std::string toString() const override { return std::to_string(value_); }
    TokenType   getTokenType() const override { return TokenType::INTEGER; }

   private:
    long long value_;
};