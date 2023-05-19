#pragma once

#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "formula_token/formula_tokens.hpp"
#include "operation/operation_proxy.hpp"

class CellTable;

class Formula {
   public:
    Formula(std::string raw_formula)
        : raw_formula_(raw_formula),
          rpn_tokeinzed_(toRPN(tokenize(raw_formula))) {}

    std::string      toString() const;
    std::string      dumpFull(CellTable* table) const;
    AbstractDataType evaluate(CellTable* table) const;

   private:
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

    std::string raw_formula_;
    TokenVec    rpn_tokeinzed_;

    TokenVec tokenize(std::string raw_formula) const;
    TokenVec toRPN(TokenVec tokens) const;

    // TODO: synchronize operators with operation factory
    static bool isOperator(char c);
    static bool isParenthesis(char c);
    static void handleOperator(
        std::string::iterator&                      it,
        std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula);
    static void handleAlpha(
        std::string::iterator& it, const std::string::iterator& end,
        std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula);
    static void handleNumeric(
        std::string::iterator& it, const std::string::iterator& end,
        std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula);
    static void handleParenthesis(
        std::string::iterator&                      it,
        std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula);
};