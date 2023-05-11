#pragma once

#include "formula_token.hpp"

class Parenthesis : public FormulaToken {
   public:
    enum class Type { OPEN, CLOSE };

    Parenthesis(Type type) : type(type) {}
    Parenthesis(char raw_parenthesis);
    Parenthesis(std::string raw_parenthesis)
        : Parenthesis(raw_parenthesis[0]) {}
    std::string toString() const override;

    Type type;
};