#pragma once

#include "formula.hpp"
#include "formula_token/formula_token.hpp"
#include "operation/operation_proxy.hpp"

class Formula;

/// @brief Pure static class that converts a formula to RPN and back
class RPNConverter {
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    static TokenVec    toRPN(const Formula& formula, TokenVec tokens);
    static std::string RPNtoString(const TokenVec& tokens);

   private:
    static void handleOperation(
        std::unique_ptr<FormulaToken>&             token,
        std::stack<std::unique_ptr<FormulaToken>>& stack, TokenVec& output);

    static void handleParenthesis(
        std::unique_ptr<FormulaToken>&             token,
        std::stack<std::unique_ptr<FormulaToken>>& stack, TokenVec& output,
        const Formula& formula);

    static void handleOperand(std::unique_ptr<FormulaToken>& token,
                              TokenVec&                      output);

    static void processRemainingStack(
        std::stack<std::unique_ptr<FormulaToken>>& stack, TokenVec& output,
        const Formula& formula);
};
