#pragma once

#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "formula_token/formula_tokens.hpp"
#include "operation/operation_factory.hpp"
#include "table.hpp"

class Formula {
   public:
    Formula(std::string raw_formula)
        : raw_formula_(raw_formula),
          rpn_tokeinzed_(toRPN(tokenize(raw_formula))) {}

    std::string      toString() const;
    std::string      dumpFull(std::shared_ptr<Table> table) const;
    AbstractDataType evaluate(std::shared_ptr<Table> table) const;

   private:
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

    std::string raw_formula_;
    TokenVec    rpn_tokeinzed_;

    TokenVec tokenize(std::string raw_formula) const;
    TokenVec toRPN(std::vector<std::unique_ptr<FormulaToken>> tokens) const;
};