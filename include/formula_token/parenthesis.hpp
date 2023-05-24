#pragma once

#include <stdexcept>

#include "formula_token.hpp"

class Parenthesis : public FormulaToken {
   public:
    enum class State { OPEN, CLOSE };

    Parenthesis(State state) : state_(state) {}
    Parenthesis(char raw_parenthesis);
    Parenthesis(std::string raw_parenthesis)
        : Parenthesis(raw_parenthesis[0]) {}

    TokenType   getTokenType() const override { return TokenType::PARENTHESIS; }
    State       getState() const { return state_; }
    std::string toString() const override;
    // NLOHMANN_DEFINE_TYPE_INTRUSIVE(Parenthesis, state_);
    friend void to_json(nlohmann::json& j, const Parenthesis& p) {
        j = nlohmann::json(p.toString());
    }

    State state_;
};