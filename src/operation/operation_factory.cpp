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

UnaryFunction cosFunc = [](const ADT& a) {
    return std::visit(CosVisitor{}, a);
};

UnaryFunction absFunc = [](const ADT& a) {
    return std::visit(AbsVisitor{}, a);
};

UnaryFunction negFunc = [](const ADT& a) {
    return std::visit(NegationVisitor{}, a);
};

// initialize static map
std::unordered_map<OperationFactory::OperationName, std::shared_ptr<Operation>>
    OperationFactory::name_to_operation_;

std::unordered_map<std::string, OperationFactory::OperationName>
    OperationFactory::unary_operation_string_to_name_ = {
        {"sin", OperationName::SIN},
        {"cos", OperationName::COS},
        {"abs", OperationName::ABS},
        {"-", OperationName::NEG}};

std::unordered_map<std::string, OperationFactory::OperationName>
    OperationFactory::binary_operation_string_to_name_ = {
        {"+", OperationName::ADD},
        {"-", OperationName::SUB},
        {"*", OperationName::MUL},
        {"/", OperationName::DIV}};

std::shared_ptr<Operation> OperationFactory::getOperation(OperationName name) {
    auto it = name_to_operation_.find(name);
    if (it != name_to_operation_.end()) return it->second;

    // operation doesn't exist, create and store it
    std::shared_ptr<Operation> operation;

    switch (name) {
        case OperationName::ADD:
            operation = std::make_shared<Operation>(addFunc, "+", 1);
            break;
        case OperationName::SUB:
            operation = std::make_shared<Operation>(subFunc, "-", 1);
            break;
        case OperationName::MUL:
            operation = std::make_shared<Operation>(mulFunc, "*", 2);
            break;
        case OperationName::DIV:
            operation = std::make_shared<Operation>(divFunc, "/", 2);
            break;
        case OperationName::SIN:
            operation = std::make_shared<Operation>(sinFunc, "sin", 3);
            break;
        case OperationName::COS:
            operation = std::make_shared<Operation>(cosFunc, "cos", 3);
            break;
        case OperationName::ABS:
            operation = std::make_shared<Operation>(absFunc, "abs", 3);
            break;
        case OperationName::NEG:
            operation = std::make_shared<Operation>(negFunc, "-", 3);
            break;
    }

    name_to_operation_[name] = operation;
    return operation;
}

OperationFactory::OperationName OperationFactory::getUnaryOperationName(
    const std::string& str) {
    auto it = unary_operation_string_to_name_.find(str);
    if (it != unary_operation_string_to_name_.end()) return it->second;

    throw std::runtime_error("Invalid unary operation name: " + str);
}

OperationFactory::OperationName OperationFactory::getBinaryOperationName(
    const std::string& str) {
    auto it = binary_operation_string_to_name_.find(str);
    if (it != binary_operation_string_to_name_.end()) return it->second;

    throw std::runtime_error("Invalid binary operation name: " + str);
}

OperationFactory::OperationName OperationFactory::getOperationName(
    const std::string& str) {
    try {
        return getUnaryOperationName(str);
    } catch (const std::runtime_error& e) {
        return getBinaryOperationName(str);
    }
}
