#pragma once

#include "formula_token.hpp"

class Parenthesis : public FormulaToken {
   public:
    enum class State { OPEN, CLOSE };

    Parenthesis(State state) : state_(state) {}
    Parenthesis(char raw_parenthesis);
    Parenthesis(std::string raw_parenthesis)
        : Parenthesis(raw_parenthesis[0]) {}

    std::string toString() const override;
    TokenType   getTokenType() const override { return TokenType::PARENTHESIS; }
    State       getState() const { return state_; }

    State state_;
};