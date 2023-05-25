#pragma once

#include "formula.hpp"
#include "formula_token/formula_token.hpp"
#include "operation/operation_proxy.hpp"

class Formula;

/// @brief Pure static class that converts a string formula to RPN and back
class RPNConverter {
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    /// @brief Converts a formula to RPN (postfix notation)
    static TokenVec    toRPN(const Formula& formula, TokenVec tokens);
    /// @brief Converts a formula from RPN (postfix notation) to infix notation,
    /// adds brackets for safety
    static std::string RPNtoString(const TokenVec& tokens);

   private:
    /// @brief Helper function for toRPN, handles an operation token
    static void handleOperation(
        std::unique_ptr<FormulaToken>&             token,
        std::stack<std::unique_ptr<FormulaToken>>& stack, TokenVec& output);

    /// @brief Helper function for toRPN, handles a parenthesis token
    static void handleParenthesis(
        std::unique_ptr<FormulaToken>&             token,
        std::stack<std::unique_ptr<FormulaToken>>& stack, TokenVec& output,
        const Formula& formula);

    /// @brief Helper function for toRPN, handles an operand token
    static void handleOperand(std::unique_ptr<FormulaToken>& token,
                              TokenVec&                      output);

    /// @brief Helper function for toRPN, processes the remaining stack after
    static void processRemainingStack(
        std::stack<std::unique_ptr<FormulaToken>>& stack, TokenVec& output,
        const Formula& formula);
};
