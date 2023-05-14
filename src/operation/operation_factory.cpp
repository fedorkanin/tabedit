#include "operation_factory.hpp"

using ADT            = AbstractDataType;
using UnaryFunction  = std::function<ADT(const ADT&)>;
using BinaryFunction = std::function<ADT(const ADT&, const ADT&)>;

BinaryFunction addFunc = [](const ADT& a, const ADT& b) {
    return std::visit(AdditionVisitor{}, a, b);
};

BinaryFunction subFunc = [](const ADT& a, const ADT& b) {
    return std::visit(SubtractionVisitor{}, a, b);
};

BinaryFunction mulFunc = [](const ADT& a, const ADT& b) {
    return std::visit(MultiplicationVisitor{}, a, b);
};

BinaryFunction divFunc = [](const ADT& a, const ADT& b) {
    return std::visit(DivisionVisitor{}, a, b);
};

std::unique_ptr<Operation> OperationFactory::getOperation(std::string name) {
    // Operation constructor is private, can't use make_unique
    if (name == "+")
        return std::unique_ptr<Operation>(new Operation(addFunc, name, 1));
    else if (name == "-")
        return std::unique_ptr<Operation>(new Operation(subFunc, name, 1));
    else if (name == "*")
        return std::unique_ptr<Operation>(new Operation(mulFunc, name, 2));
    else if (name == "/")
        return std::unique_ptr<Operation>(new Operation(divFunc, name, 2));

    throw std::runtime_error("Invalid operation name.");
}