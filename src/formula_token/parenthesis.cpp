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

using json = nlohmann::json;
json Parenthesis::toJSON() const {
    json j;
    j["type"]  = "parenthesis";
    j["state"] = state_ == State::OPEN ? "open" : "close";
    return j;
}
