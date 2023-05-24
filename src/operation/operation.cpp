#include "operation.hpp"

using ADT            = AbstractDataType;
using UnaryFunction  = std::function<ADT(const ADT&)>;
using BinaryFunction = std::function<ADT(const ADT&, const ADT&)>;

ADT Operation::executeUnary(const ADT& a) const {
    return std::visit(
        VisitorOverloader{
            [&a](const UnaryFunction& op) { return op(a); },
            [](const BinaryFunction&) -> ADT {
                throw std::runtime_error(
                    "Invalid operation type for unary operation.");
            }},
        function_);
}

ADT Operation::executeBinary(const ADT& a, const ADT& b) const {
    return std::visit(
        VisitorOverloader{
            [](const UnaryFunction&) -> ADT {
                throw std::runtime_error(
                    "Invalid operation type for binary operation.");
            },
            [&a, &b](const BinaryFunction& op) { return op(a, b); }},
        function_);
}

ADT Operation::execute(const ADT& a) const {
    if (arity_ != 1)
        throw std::runtime_error("Invalid arity for unary operation.");
    return executeUnary(a);
}

ADT Operation::execute(const ADT& a, const ADT& b) const {
    if (arity_ != 2)
        throw std::runtime_error("Invalid arity for binary operation.");
    return executeBinary(a, b);
}

ADT Operation::execute(const std::vector<ADT>& args) const {
    if (args.size() != arity_)
        throw std::runtime_error("Invalid arity for operation.");
    if (arity_ == 1)
        return executeUnary(args[0]);
    else
        return executeBinary(args[1], args[0]);
}
