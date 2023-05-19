#include "formula.hpp"

std::vector<std::unique_ptr<FormulaToken>> Formula::toRPN(
    std::vector<std::unique_ptr<FormulaToken>> tokens) const {
    std::vector<std::unique_ptr<FormulaToken>> output;
    std::stack<std::unique_ptr<FormulaToken>>  stack;

    for (auto& token : tokens) {
        if (token->getTokenType() == FormulaToken::TokenType::OPERATION) {
            // Handle operations
            while (
                !stack.empty() &&
                stack.top()->getTokenType() ==
                    FormulaToken::TokenType::OPERATION &&
                static_cast<OperationProxy*>(stack.top().get())
                        ->getPriority() >=
                    static_cast<OperationProxy*>(token.get())->getPriority()) {
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
    std::string::iterator                      it = raw_formula.begin();
    std::vector<std::unique_ptr<FormulaToken>> tokenized_formula;

    while (it != raw_formula.end()) {
        if (isOperator(*it)) {
            handleOperator(it, tokenized_formula);
        } else if (std::isalpha(*it)) {
            handleAlpha(it, raw_formula.end(), tokenized_formula);
        } else if (std::isdigit(*it)) {
            handleNumeric(it, raw_formula.end(), tokenized_formula);
        } else if (isParenthesis(*it)) {
            handleParenthesis(it, tokenized_formula);
        } else if (std::isspace(*it)) {
            ++it;
        } else {
            throw std::runtime_error("Unknown token: " + std::string(1, *it));
        }
    }

    return tokenized_formula;
}

bool Formula::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool Formula::isParenthesis(char c) { return c == '(' || c == ')'; }

void Formula::handleOperator(
    std::string::iterator&                      it,
    std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula) {
    tokenized_formula.emplace_back(
        std::make_unique<OperationProxy>(std::string(1, *it)));
    ++it;
}

void Formula::handleAlpha(
    std::string::iterator& it, const std::string::iterator& end,
    std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula) {
    std::string function_name;
    while (it != end && std::isalpha(*it)) {
        function_name.push_back(*it);
        ++it;
    }

    if (function_name == "sin" || function_name == "cos" ||
        function_name == "tan" || function_name == "log" ||
        function_name == "sqrt") {
        tokenized_formula.emplace_back(
            std::make_unique<OperationProxy>(function_name));
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
}

void Formula::handleNumeric(
    std::string::iterator& it, const std::string::iterator& end,
    std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula) {
    std::string number;
    while (it != end && (std::isdigit(*it) || *it == '.')) {
        number.push_back(*it);
        ++it;
    }
    if (number.find('.') != std::string::npos) {
        tokenized_formula.emplace_back(std::make_unique<Double>(number));
    } else {
        tokenized_formula.emplace_back(std::make_unique<Integer>(number));
    }
}

void Formula::handleParenthesis(
    std::string::iterator&                      it,
    std::vector<std::unique_ptr<FormulaToken>>& tokenized_formula) {
    tokenized_formula.push_back(std::make_unique<Parenthesis>(*it));
    ++it;
}

std::string Formula::toString() const { return raw_formula_; }

std::string Formula::dumpFull(CellTable* table) const {
    std::string output = "Formula: " + raw_formula_ + "\n";
    output += "RPN: ";
    for (auto& token : rpn_tokeinzed_) {
        output += token->toString() + " ";
    }
    output += "\n";
    output +=
        "Evaluation: " + std::visit(ToStringVisitor(), evaluate(table)) + "\n";
    return output;
}

AbstractDataType Formula::evaluate(CellTable* table) const {
    std::stack<AbstractDataType> stack;
    for (auto& token : rpn_tokeinzed_) {
        OperationProxy* operation = nullptr;
        switch (token->getTokenType()) {
            case FormulaToken::TokenType::OPERATION:
                operation = static_cast<OperationProxy*>(token.get());
                if (operation->getArity() == 1) {
                    if (stack.size() < 1)
                        throw std::runtime_error("Invalid formula");
                    AbstractDataType operand = stack.top();
                    stack.pop();
                    stack.push(operation->execute(operand));
                } else {
                    if (stack.size() < 2)
                        throw std::runtime_error("Invalid formula");
                    AbstractDataType operand2 = stack.top();
                    stack.pop();
                    AbstractDataType operand1 = stack.top();
                    stack.pop();
                    stack.push(operation->execute(operand1, operand2));
                }
                break;
            case FormulaToken::TokenType::CELL_COORD:
                // TODO: Handle cell coordinates
                throw std::runtime_error(
                    "Cell coordinate evaluation not implemented");
                break;
            case FormulaToken::TokenType::INTEGER:
                stack.push(
                    Integer(static_cast<Integer*>(token.get())->getValue()));
                break;
            case FormulaToken::TokenType::DOUBLE:
                stack.push(
                    Double(static_cast<Double*>(token.get())->getValue()));
                break;
            case FormulaToken::TokenType::STRING:
                stack.push(
                    String(static_cast<String*>(token.get())->getValue()));
            case FormulaToken::TokenType::PARENTHESIS:
                throw std::runtime_error("Invalid formula");
                break;
        }
    }
    if (stack.size() != 1) {
        throw std::runtime_error("Invalid formula");
    }
    return stack.top();
}
