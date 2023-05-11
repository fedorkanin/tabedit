#include "formula.hpp"

void Formula::tokenize() {
    std::string                                token;
    std::string::iterator                      it = raw_formula_.begin();
    std::vector<std::unique_ptr<FormulaToken>> tokens;

    while (it != raw_formula_.end()) {
        if (*it == '+' || *it == '-' || *it == '*' || *it == '/' ||
            *it == '^') {
            // Handle arithmetic operations
            tokens.push_back(std::make_unique<Operation>(std::string(1, *it)));
            ++it;
        } else if (std::isalpha(*it)) {
            // Handle function names or cell coordinates
            std::string function;
            while (it != raw_formula_.end() && std::isalpha(*it)) {
                function.push_back(*it);
                ++it;
            }
            if (function == "sin") {
                tokens.push_back(
                    std::make_unique<Operation>(Operation::Type::SIN));
            } else if (function == "cos") {
                tokens.push_back(
                    std::make_unique<Operation>(Operation::Type::COS));
            } else if (function == "abs") {
                tokens.push_back(
                    std::make_unique<Operation>(Operation::Type::ABS));
            } else {
                if (CellCoord::isValidCoord(function)) {
                    tokens.push_back(std::make_unique<CellCoord>(function));
                } else {
                    throw std::runtime_error(
                        "Unknown function or invalid cell coordinate: " +
                        function);
                }
            }
        } else if (std::isdigit(*it)) {
            // Handle numeric values
            std::string number;
            while (it != raw_formula_.end() &&
                   (std::isdigit(*it) || *it == '.')) {
                number.push_back(*it);
                ++it;
            }
            if (number.find('.') != std::string::npos) {
                tokens.push_back(std::make_unique<Double>(std::stod(number)));
            } else {
                tokens.push_back(std::make_unique<Integer>(std::stoi(number)));
            }
        } else if (*it == '(') {
            // Handle opening parenthesis
            tokens.push_back(std::make_unique<Parenthesis>(*it));
            ++it;
        } else if (*it == ')') {
            // Handle closing parenthesis
            tokens.push_back(std::make_unique<Parenthesis>(*it));
            ++it;
        } else if (std::isspace(*it)) {
            // Skip whitespace
            ++it;
        } else {
            // Unknown token
            throw std::runtime_error("Unknown token: " + std::string(1, *it));
        }
    }

    tokenized_formula_ = std::move(tokens);
}

std::string Formula::toString() const { return raw_formula_; }

std::string Formula::dumpTokenized() const {
    std::string dump;
    for (const auto& token : tokenized_formula_) {
        dump += token->toString() + " ";
    }
    return dump;
}
