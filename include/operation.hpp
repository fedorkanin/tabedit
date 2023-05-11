#pragma once

#include "formula_token.hpp"

class Operation : public FormulaToken {
   public:
    enum class Type { SUM, SUB, MUL, DIV, POW, SIN, COS, ABS };

    Operation(Type type) : type(type) {}
    Operation(std::string raw_operation);
    std::string toString() const override;

    Type type;
};