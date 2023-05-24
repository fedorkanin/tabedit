#include "formula.hpp"
using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

std::set<CellCoord> Formula::getReferencedCells() const {
    std::set<CellCoord> referenced_cells;
    for (auto& token : rpn_tokeinzed_) {
        if (token->getTokenType() == FormulaToken::TokenType::CELL_COORD) {
            referenced_cells.insert(*static_cast<CellCoord*>(token.get()));
        }
    }
    return referenced_cells;
}

std::string Formula::toString() const { return RPNtoString(rpn_tokeinzed_); }

TokenVec Formula::tokenize(std::string raw_formula) const {
    std::string::iterator it = raw_formula.begin();
    while (it != raw_formula.end() && std::isspace(*it)) ++it;
    TokenVec tokenized_formula;

    while (it != raw_formula.end()) {
        if (isSimpleOperator(*it)) {
            handleSimpleOperator(it, tokenized_formula, raw_formula);
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

TokenVec Formula::toRPN(TokenVec tokens) const {
    return RPNConverter::toRPN(*this, std::move(tokens));
}

std::string Formula::RPNtoString(const TokenVec& tokens) const {
    return RPNConverter::RPNtoString(tokens);
}

bool Formula::isSimpleOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool Formula::isParenthesis(char c) { return c == '(' || c == ')'; }

void Formula::handleSimpleOperator(std::string::iterator& it,
                                   TokenVec&              tokenized_formula,
                                   const std::string&     raw_formula) {
    // '-' can be unary or binary operator
    if (*it == '-') {
        bool is_unary_minus = false;
        if (it == raw_formula.begin()) {
            is_unary_minus = true;
        } else {
            auto prev_char_it = it;
            for (--prev_char_it; std::isspace(*prev_char_it); --prev_char_it)
                ;

            is_unary_minus = true;
        }

        if (is_unary_minus) {
            tokenized_formula.emplace_back(std::make_unique<OperationProxy>(
                OperationFactory::getUnaryOperationName("-")));
        } else {
            tokenized_formula.emplace_back(std::make_unique<OperationProxy>(
                OperationFactory::getBinaryOperationName("-")));
        }
    } else {
        tokenized_formula.emplace_back(std::make_unique<OperationProxy>(
            OperationFactory::getBinaryOperationName(std::string(1, *it))));
    }
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

    if (raw_token == "sin" || raw_token == "cos" || raw_token == "abs") {
        auto operation_name =
            OperationFactory::getUnaryOperationName(raw_token);
        tokenized_formula.emplace_back(
            std::make_unique<OperationProxy>(operation_name));
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
