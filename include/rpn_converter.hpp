#pragma once

#include "formula.hpp"
#include "formula_token/formula_token.hpp"
#include "operation/operation_proxy.hpp"

class Formula;

namespace RPNConverter {
using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

void handleOperation(std::unique_ptr<FormulaToken>&             token,
                     std::stack<std::unique_ptr<FormulaToken>>& stack,
                     TokenVec&                                  output);

void handleParenthesis(std::unique_ptr<FormulaToken>&             token,
                       std::stack<std::unique_ptr<FormulaToken>>& stack,
                       TokenVec& output, const Formula& formula);

void handleOperand(std::unique_ptr<FormulaToken>& token, TokenVec& output);

void processRemainingStack(std::stack<std::unique_ptr<FormulaToken>>& stack,
                           TokenVec& output, const Formula& formula);

TokenVec toRPN(const Formula& formula, TokenVec tokens);

std::string RPNtoString(const TokenVec& tokens);
}  // namespace RPNConverter
