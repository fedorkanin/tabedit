#pragma once

#include <memory>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "../libs/json.hpp"
#include "formula_token/formula_tokens.hpp"
#include "operation/operation_proxy.hpp"

class CellTable;

class Formula {
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    Formula(std::string raw_formula)
        : rpn_tokeinzed_(toRPN(tokenize(raw_formula))) {}

    /// @todo: implement toString
    std::string     dumpFull(CellTable* table) const;
    const TokenVec& getRPN() const { return rpn_tokeinzed_; }
    bool            isDependentOn(const CellCoord& coord) const {
        return dependent_on_.find(coord) != dependent_on_.end();
    }
    std::set<CellCoord> getReferencedCells() const;
    std::string         toString() const;

    friend void to_json(nlohmann::json& j, const Formula& f) {
        j = f.toString();
    }

   private:
    mutable TokenVec    rpn_tokeinzed_;
    std::set<CellCoord> dependent_on_;

    TokenVec    tokenize(std::string raw_formula) const;
    TokenVec    toRPN(TokenVec tokens) const;
    std::string RPNtoNormal(const TokenVec& tokens) const;

    /// @todo: synchronize operators with operation factory
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