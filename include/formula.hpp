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
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    Formula(std::string raw_formula)
        : rpn_tokeinzed_(toRPN(tokenize(raw_formula))) {}

    // TODO: implement toString
    std::string     toString() const { return "TOSTRING NOT IMPLEMENTED"; }
    std::string     dumpFull(CellTable* table) const;
    const TokenVec& getRPN() const { return rpn_tokeinzed_; }

   private:
    mutable TokenVec rpn_tokeinzed_;

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
    static void handleString(
        std::string::iterator& it, const std::string::iterator& end,
        std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula);
};