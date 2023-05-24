#include "rpn_converter.hpp"

namespace RPNConverter {
void handleOperation(std::unique_ptr<FormulaToken>&             token,
                     std::stack<std::unique_ptr<FormulaToken>>& stack,
                     TokenVec&                                  output) {
    while (!stack.empty() &&
           stack.top()->getTokenType() == FormulaToken::TokenType::OPERATION &&
           static_cast<OperationProxy*>(stack.top().get())->getPriority() >=
               static_cast<OperationProxy*>(token.get())->getPriority()) {
        output.push_back(std::move(stack.top()));
        stack.pop();
    }
    stack.push(std::move(token));
}

void handleParenthesis(std::unique_ptr<FormulaToken>&             token,
                       std::stack<std::unique_ptr<FormulaToken>>& stack,
                       TokenVec& output, const Formula& formula) {
    if (static_cast<Parenthesis*>(token.get())->getState() ==
        Parenthesis::State::OPEN) {
        stack.push(std::move(token));
    } else {
        while (!stack.empty() && stack.top()->getTokenType() !=
                                     FormulaToken::TokenType::PARENTHESIS) {
            output.push_back(std::move(stack.top()));
            stack.pop();
        }
        if (stack.empty()) {
            throw std::runtime_error("Mismatched parenthesis in formula: " +
                                     formula.toString());
        }
        stack.pop();
    }
}

void handleOperand(std::unique_ptr<FormulaToken>& token, TokenVec& output) {
    output.push_back(std::move(token));
}

void processRemainingStack(std::stack<std::unique_ptr<FormulaToken>>& stack,
                           TokenVec& output, const Formula& formula) {
    while (!stack.empty()) {
        if (stack.top()->getTokenType() ==
            FormulaToken::TokenType::PARENTHESIS) {
            throw std::runtime_error("Mismatched parenthesis in formula: " +
                                     formula.toString());
        }
        output.push_back(std::move(stack.top()));
        stack.pop();
    }
}

TokenVec toRPN(const Formula& formula, TokenVec tokens) {
    TokenVec                                  output;
    std::stack<std::unique_ptr<FormulaToken>> stack;

    for (auto& token : tokens) {
        if (token->getTokenType() == FormulaToken::TokenType::OPERATION) {
            handleOperation(token, stack, output);
        } else if (token->getTokenType() ==
                   FormulaToken::TokenType::PARENTHESIS) {
            handleParenthesis(token, stack, output, formula);
        } else {
            handleOperand(token, output);
        }
    }

    processRemainingStack(stack, output, formula);

    return output;
}

std::string RPNtoString(const TokenVec& tokens) {
    std::stack<std::string> stack;

    for (auto& token : tokens) {
        if (token->getTokenType() == FormulaToken::TokenType::OPERATION) {
            int arity = static_cast<OperationProxy*>(token.get())->getArity();
            if (stack.size() < static_cast<size_t>(arity)) {
                throw std::runtime_error(
                    "Invalid RPN expression: insufficient operands for "
                    "operation");
            }

            std::vector<std::string> operands(arity);
            for (int i = arity - 1; i >= 0; --i) {
                operands[i] = std::move(stack.top());
                stack.pop();
            }

            std::string expression;
            if (arity == 1) {
                expression = token->toString() + " " + operands[0];
            } else if (arity == 2) {
                expression = "(" + operands[0] + " " + token->toString() + " " +
                             operands[1] + ")";
            }

            stack.push(std::move(expression));

        } else {
            stack.push(token->toString());
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid RPN expression: too many operands");
    }

    return stack.top();
}
}  // namespace RPNConverter
