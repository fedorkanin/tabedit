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
#include "rpn_converter.hpp"

class CellTable;

class Formula {
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    Formula(std::string raw_formula)
        : rpn_tokeinzed_(toRPN(tokenize(raw_formula))) {}

    const TokenVec&     getRPN() const { return rpn_tokeinzed_; }

    /// @brief Get the cells referenced by the formula
    std::set<CellCoord> getReferencedCells() const;

    /// @brief Find if the formula is dependent on a cell, i.e. if the cell is
    /// referenced by the formula
    bool                isDependentOn(const CellCoord& coord) const;

    std::string         toString() const;
    friend void         to_json(nlohmann::json& j, const Formula& f);
    std::string         dumpFull(CellTable* table) const;

    static ADT          parsePrimitive(std::string raw_value);

   private:
    mutable TokenVec    rpn_tokeinzed_;
    std::set<CellCoord> dependent_on_;

    TokenVec            tokenize(std::string raw_formula) const;
    TokenVec            toRPN(TokenVec tokens) const;
    std::string         RPNtoString(const TokenVec& tokens) const;

    static bool         isSimpleOperator(char c);
    static bool         isParenthesis(char c);
    /// @brief Create an operation and push it to the stack, minus is checked
    /// for unarity
    static void         handleSimpleOperator(std::string::iterator& it,
                                             TokenVec&              tokenized_formula,
                                             const std::string&     raw_formula);
    static void         handleAlpha(std::string::iterator&       it,
                                    const std::string::iterator& end,
                                    TokenVec&                    tokenized_formula);
    static void         handleNumeric(std::string::iterator&       it,
                                      const std::string::iterator& end,
                                      TokenVec&                    tokenized_formula);
    static void         handleParenthesis(std::string::iterator& it,
                                          TokenVec&              tokenized_formula);
    static void         handleString(std::string::iterator&       it,
                                     const std::string::iterator& end,
                                     TokenVec&                    tokenized_formula);
};