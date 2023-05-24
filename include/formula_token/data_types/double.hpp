#pragma once

#include <string>

#include "../formula_token.hpp"

class Double : public FormulaToken {
   public:
    Double(float value) : value_(value) {}
    Double(std::string value) : value_(std::stod(value)) {}

    double getValue() const { return value_; }

    TokenType   getTokenType() const override { return TokenType::DOUBLE; }
    std::string toString() const override { return std::to_string(value_); }
    friend void to_json(nlohmann::json& j, const Double& double_) {
        j = std::to_string(double_.value_);
    }

   private:
    double value_;
};