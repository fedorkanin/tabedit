#pragma once

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "../formula_token.hpp"

#define DOUBLE_PRECISION 3

class Double : public FormulaToken {
   public:
    Double(float value) : value_(value) {}
    Double(std::string value) : value_(std::stod(value)) {}

    double      getValue() const { return value_; }
    TokenType   getTokenType() const override { return TokenType::DOUBLE; }

    std::string toString() const override;
    friend void to_json(nlohmann::json& j, const Double& double_);

   private:
    double value_;
};