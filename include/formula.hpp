#pragma once

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "cell_coord.hpp"
#include "data_types/double.hpp"
#include "data_types/integer.hpp"
#include "formula_token.hpp"
#include "operation/operation.hpp"
#include "operation/operation_factory.hpp"
#include "parenthesis.hpp"

class Formula {
   public:
    Formula(std::string raw_formula)
        : raw_formula_(raw_formula),
          rpn_tokeinzed_(toRPN(tokenize(raw_formula))) {}

    std::string toString() const;
    std::string dumpFull() const;

   private:
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

    std::string raw_formula_;
    TokenVec    rpn_tokeinzed_;

    TokenVec tokenize(std::string raw_formula) const;
    TokenVec toRPN(std::vector<std::unique_ptr<FormulaToken>> tokens) const;
};