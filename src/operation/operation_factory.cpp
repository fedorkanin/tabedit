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

UnaryFunction sinFunc = [](const ADT& a) {
    return std::visit(SinVisitor{}, a);
};

std::shared_ptr<Operation> OperationFactory::getOperation(std::string name) {
    // Operation constructor is private, can't use make_unique
    if (name == "+")
        return std::shared_ptr<Operation>(new Operation(addFunc, name, 1));
    if (name == "-")
        return std::shared_ptr<Operation>(new Operation(subFunc, name, 1));
    if (name == "*")
        return std::shared_ptr<Operation>(new Operation(mulFunc, name, 2));
    if (name == "/")
        return std::shared_ptr<Operation>(new Operation(divFunc, name, 2));
    if (name == "sin")
        return std::shared_ptr<Operation>(new Operation(sinFunc, name, 3));

    throw std::invalid_argument("OperationFactory::getOperation: " + name +
                                " is not a valid operation");
}