#include "parenthesis.hpp"

Parenthesis::Parenthesis(char raw_parenthesis) {
    switch (raw_parenthesis) {
        case '(':
            state_ = State::OPEN;
            break;
        case ')':
            state_ = State::CLOSE;
            break;
        default:
            throw std::runtime_error("Unknown parenthesis");
    }
}

std::string Parenthesis::toString() const {
    return state_ == State::OPEN ? "(" : ")";
}

void to_json(nlohmann::json& j, const Parenthesis& p) {
    j = nlohmann::json(p.toString());
}
