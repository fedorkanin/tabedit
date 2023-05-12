#include "operation/operation.hpp"

using ADT            = AbstractDataType;
using ADTptr         = std::unique_ptr<ADT>;
using UnaryFunction  = std::function<ADTptr(const ADT&)>;
using BinaryFunction = std::function<ADTptr(const ADT&, const ADT&)>;

ADTptr Operation::executeUnary(const ADT& a) const {
    return std::visit(
        VisitorOverloader{
            [&a](const UnaryFunction& op) { return op(a); },
            [](const BinaryFunction&) -> ADTptr {
                throw std::runtime_error(
                    "Invalid operation type for unary operation.");
            }},
        function_);
}

ADTptr Operation::executeBinary(const ADT& a, const ADT& b) const {
    return std::visit(
        VisitorOverloader{
            [](const UnaryFunction&) -> ADTptr {
                throw std::runtime_error(
                    "Invalid operation type for binary operation.");
            },
            [&a, &b](const BinaryFunction& op) { return op(a, b); }},
        function_);
}

ADTptr Operation::execute(const ADT& a) const {
    if (arity_ != 1)
        throw std::runtime_error("Invalid arity for unary operation.");
    return executeUnary(a);
}

ADTptr Operation::execute(const ADT& a, const ADT& b) const {
    if (arity_ != 2)
        throw std::runtime_error("Invalid arity for binary operation.");
    return executeBinary(a, b);
}
