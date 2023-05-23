#include "formula.hpp"
using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

/// @todo: Rewrite exception messages
TokenVec Formula::toRPN(TokenVec tokens) const {
    TokenVec                                  output;
    std::stack<std::unique_ptr<FormulaToken>> stack;

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
                        "Mismatched parenthesis in formula: ");
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
            throw std::runtime_error("Mismatched parenthesis in formula: ");
        }
        output.push_back(std::move(stack.top()));
        stack.pop();
    }

    return output;
}

std::set<CellCoord> Formula::getReferencedCells() const {
    std::set<CellCoord> referenced_cells;
    for (auto& token : rpn_tokeinzed_) {
        if (token->getTokenType() == FormulaToken::TokenType::CELL_COORD) {
            referenced_cells.insert(*static_cast<CellCoord*>(token.get()));
        }
    }
    return referenced_cells;
}

using json = nlohmann::json;
json Formula::toJSON() const {
    json j;
    for (auto& token : rpn_tokeinzed_) {
        switch (token->getTokenType()) {
            case FormulaToken::TokenType::OPERATION:
                j["operation"] =
                    static_cast<OperationProxy*>(token.get())->toJSON();
                break;
            case FormulaToken::TokenType::PARENTHESIS:
                j["parenthesis"] =
                    static_cast<Parenthesis*>(token.get())->toJSON();
                break;
            case FormulaToken::TokenType::CELL_COORD:
                j["cell_coord"] =
                    static_cast<CellCoord*>(token.get())->toJSON();
                break;
            case FormulaToken::TokenType::STRING:
                j["string"] = static_cast<String*>(token.get())->toJSON();
                break;
            case FormulaToken::TokenType::INTEGER:
                j["integer"] = static_cast<Integer*>(token.get())->toJSON();
                break;
            case FormulaToken::TokenType::DOUBLE:
                j["double"] = static_cast<Double*>(token.get())->toJSON();
                break;
            case FormulaToken::TokenType::NOTYPE:
                j["notype"] = static_cast<NoType*>(token.get())->toJSON();
                break;
        }
    }
    return j;
}

TokenVec Formula::tokenize(std::string raw_formula) const {
    std::string::iterator it = raw_formula.begin();
    TokenVec              tokenized_formula;
    // if (raw_formula[0] != '=')
    //     throw std::runtime_error("Formula must start with '='");
    // ++it;

    while (it != raw_formula.end()) {
        if (isOperator(*it)) {
            handleOperator(it, tokenized_formula);
        } else if (std::isalpha(*it)) {
            handleAlpha(it, raw_formula.end(), tokenized_formula);
        } else if (*it == '\"') {
            handleString(it, raw_formula.end(), tokenized_formula);
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

void Formula::handleOperator(std::string::iterator& it,
                             TokenVec&              tokenized_formula) {
    tokenized_formula.emplace_back(
        std::make_unique<OperationProxy>(std::string(1, *it)));
    ++it;
}

void Formula::handleAlpha(std::string::iterator&       it,
                          const std::string::iterator& end,
                          TokenVec&                    tokenized_formula) {
    std::string raw_token;
    while (it != end && (std::isalpha(*it) || *it == '_')) {
        raw_token.push_back(*it);
        ++it;
    }

    if (raw_token.empty())
        throw std::runtime_error("Invalid token: " + raw_token);

    if (raw_token == "sin" || raw_token == "cos" || raw_token == "tan" ||
        raw_token == "log" || raw_token == "sqrt") {
        tokenized_formula.emplace_back(
            std::make_unique<OperationProxy>(raw_token));
        return;
    }

    while (it != end && std::isdigit(*it)) {
        raw_token.push_back(*it);
        ++it;
    }

    if (CellCoord::isValidCoord(raw_token))
        tokenized_formula.emplace_back(std::make_unique<CellCoord>(raw_token));
    else
        throw std::runtime_error("Unknown token in handleAlpha: " + raw_token);
}

void Formula::handleNumeric(std::string::iterator&       it,
                            const std::string::iterator& end,
                            TokenVec&                    tokenized_formula) {
    std::string number;
    while (it != end && (std::isdigit(*it) || *it == '.')) {
        number.push_back(*it);
        ++it;
    }

    if (number.find('.') != std::string::npos)
        tokenized_formula.emplace_back(std::make_unique<Double>(number));
    else
        tokenized_formula.emplace_back(std::make_unique<Integer>(number));
}

void Formula::handleParenthesis(std::string::iterator& it,
                                TokenVec&              tokenized_formula) {
    tokenized_formula.emplace_back(std::make_unique<Parenthesis>(*it));
    ++it;
}

void Formula::handleString(std::string::iterator&       it,
                           const std::string::iterator& end,
                           TokenVec&                    tokenized_formula) {
    std::string raw_token;
    ++it;
    bool escaped = false;
    while (it != end && (*it != '"' || escaped)) {
        if (*it == '\\') {
            escaped = true;
        } else {
            escaped = false;
        }
        raw_token.push_back(*it);
        ++it;
    }
    if (it == end) throw std::runtime_error("Invalid string: " + raw_token);
    ++it;
    tokenized_formula.emplace_back(std::make_unique<String>(raw_token));
}

std::string Formula::dumpFull(CellTable* table) const {
    std::string output = "RPN: ";
    for (auto& token : rpn_tokeinzed_) {
        output += token->toString() + " ";
    }
    output += "\n";

    return output;
}
