#pragma once

#include <string>

#include "../formula_token.hpp"

class NoType : public FormulaToken {
   public:
    std::string toString() const override { return "#REF!"; }
    TokenType   getTokenType() const override { return TokenType::NOTYPE; }
    // NLOHMANN_DEFINE_TYPE_INTRUSIVE(NoType, "not");
    friend void to_json(nlohmann::json& j, const NoType& p) {
        j = nlohmann::json("notype");
    }
};