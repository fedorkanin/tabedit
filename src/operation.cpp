#include "operation.hpp"

Operation::Operation(std::string raw_operation) {
    if (raw_operation == "+") {
        type = Type::SUM;
    } else if (raw_operation == "-") {
        type = Type::SUB;
    } else if (raw_operation == "*") {
        type = Type::MUL;
    } else if (raw_operation == "/") {
        type = Type::DIV;
    } else if (raw_operation == "^") {
        type = Type::POW;
    } else if (raw_operation == "sin") {
        type = Type::SIN;
    } else if (raw_operation == "cos") {
        type = Type::COS;
    } else if (raw_operation == "abs") {
        type = Type::ABS;
    } else {
        throw std::runtime_error("Unknown operation");
    }
}

std::string Operation::toString() const {
    switch (type) {
        case Type::SUM:
            return "+";
        case Type::SUB:
            return "-";
        case Type::MUL:
            return "*";
        case Type::DIV:
            return "/";
        case Type::POW:
            return "^";
        case Type::SIN:
            return "sin";
        case Type::COS:
            return "cos";
        case Type::ABS:
            return "abs";
    }
    throw std::runtime_error("Unknown operation type");
}