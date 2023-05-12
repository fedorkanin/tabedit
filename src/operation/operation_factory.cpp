#include "operation/operation_factory.hpp"

using ADT            = AbstractDataType;
using ADTptr         = std::unique_ptr<ADT>;
using UnaryFunction  = std::function<ADTptr(const ADT&)>;
using BinaryFunction = std::function<ADTptr(const ADT&, const ADT&)>;

BinaryFunction addition = [](const ADT& a, const ADT& b) {
    return a.getSum(b);
};

std::unique_ptr<Operation> OperationFactory::getOperation(std::string name) {
    if (name == "+") return std::make_unique<Operation>(addition, name, 1);

    throw std::runtime_error("Invalid operation name.");
}