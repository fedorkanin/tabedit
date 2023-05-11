#include "parenthesis.hpp"

Parenthesis::Parenthesis(char raw_parenthesis) {
    switch (raw_parenthesis) {
        case '(':
            type = Type::OPEN;
            break;
        case ')':
            type = Type::CLOSE;
            break;
        default:
            throw std::runtime_error("Unknown parenthesis");
    }
}

std::string Parenthesis::toString() const {
    return type == Type::OPEN ? "(" : ")";
}
