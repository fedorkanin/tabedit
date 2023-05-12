#include "formula.hpp"

std::vector<std::unique_ptr<FormulaToken>> Formula::toRPN(
    std::vector<std::unique_ptr<FormulaToken>> tokens) const {
    std::vector<std::unique_ptr<FormulaToken>> output;
    std::stack<std::unique_ptr<FormulaToken>>  stack;

    for (auto& token : tokens) {
        if (token->getTokenType() == FormulaToken::TokenType::OPERATION) {
            // Handle operations
            while (!stack.empty() &&
                   stack.top()->getTokenType() ==
                       FormulaToken::TokenType::OPERATION &&
                   static_cast<Operation*>(stack.top().get())->getPriority() >=
                       static_cast<Operation*>(token.get())->getPriority()) {
                output.push_back(std::move(stack.top()));
                stack.pop();
            }
            stack.push(std::move(token));
        } else if (token->getTokenType() ==
                   FormulaToken::TokenType::PARENTHESIS) {
            // Handle parenthesis
            if (static_cast<Parenthesis*>(token.get())->getState() ==
                Parenthesis::State::OPEN) {
                stack.push(std::move(token));
            } else {
                while (!stack.empty() &&
                       stack.top()->getTokenType() !=
                           FormulaToken::TokenType::PARENTHESIS) {
                    output.push_back(std::move(stack.top()));
                    stack.pop();
                }
                if (stack.empty()) {
                    throw std::runtime_error(
                        "Mismatched parenthesis in formula: " + raw_formula_);
                }
                stack.pop();
            }
        } else {
            // Handle operands
            output.push_back(std::move(token));
        }
    }

    while (!stack.empty()) {
        if (stack.top()->getTokenType() ==
            FormulaToken::TokenType::PARENTHESIS) {
            throw std::runtime_error("Mismatched parenthesis in formula: " +
                                     raw_formula_);
        }
        output.push_back(std::move(stack.top()));
        stack.pop();
    }

    return output;
}

std::vector<std::unique_ptr<FormulaToken>> Formula::tokenize(
    std::string raw_formula) const {
    std::string                                token;
    std::string::iterator                      it = raw_formula.begin();
    std::vector<std::unique_ptr<FormulaToken>> tokenized_formula;

    while (it != raw_formula.end()) {
        if (*it == '+' || *it == '-' || *it == '*' || *it == '/' ||
            *it == '^') {
            // Handle arithmetic operations
            tokenized_formula.emplace_back(OperationFactory::getOperation(*it));
            ++it;
        } else if (std::isalpha(*it)) {
            // Handle function names or cell coordinates
            std::string function_name;
            while (it != raw_formula.end() && std::isalpha(*it)) {
                function_name.push_back(*it);
                ++it;
            }
            if (function_name == "sin" || function_name == "cos" ||
                function_name == "tan" || function_name == "log" ||
                function_name == "sqrt") {
                tokenized_formula.emplace_back(
                    OperationFactory::getOperation(function_name));
            } else {
                if (CellCoord::isValidCoord(function_name)) {
                    tokenized_formula.push_back(
                        std::make_unique<CellCoord>(function_name));
                } else {
                    throw std::runtime_error(
                        "Unknown function or invalid cell coordinate: " +
                        function_name);
                }
            }
        } else if (std::isdigit(*it)) {
            // Handle numeric values
            std::string number;
            while (it != raw_formula.end() &&
                   (std::isdigit(*it) || *it == '.')) {
                number.push_back(*it);
                ++it;
            }
            if (number.find('.') != std::string::npos) {
                tokenized_formula.push_back(
                    std::make_unique<Double>(std::stod(number)));
            } else {
                tokenized_formula.push_back(
                    std::make_unique<Integer>(std::stoi(number)));
            }
        } else if (*it == '(') {
            // Handle opening parenthesis
            tokenized_formula.push_back(std::make_unique<Parenthesis>(*it));
            ++it;
        } else if (*it == ')') {
            // Handle closing parenthesis
            tokenized_formula.push_back(std::make_unique<Parenthesis>(*it));
            ++it;
        } else if (std::isspace(*it)) {
            // Skip whitespace
            ++it;
        } else {
            // Unknown token
            throw std::runtime_error("Unknown token: " + std::string(1, *it));
        }
    }

    return tokenized_formula;
}

std::string Formula::toString() const { return raw_formula_; }

std::string Formula::dumpFull() const {
    std::string output = "Formula: " + raw_formula_ + "\n";
    output += "RPN: ";
    for (auto& token : rpn_tokeinzed_) {
        output += token->toString() + " ";
    }
    output += "\n";
    return output;
}
