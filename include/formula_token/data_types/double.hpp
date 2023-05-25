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
    std::string toString() const override {
        double rounded = std::round(value_ * pow(10, DOUBLE_PRECISION)) /
                         pow(10, DOUBLE_PRECISION);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(DOUBLE_PRECISION) << rounded;
        return ss.str();
    }
    friend void to_json(nlohmann::json& j, const Double& double_) {
        j = double_.toString();
    }

   private:
    double value_;
};