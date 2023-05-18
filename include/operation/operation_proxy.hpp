#pragma once

#include "../formula_token/formula_tokens.hpp"
#include "operation.hpp"
#include "operation_factory.hpp"

class OperationProxy : public FormulaToken {
   public:
    OperationProxy(std::string name)
        : operation_(OperationFactory::getOperation(name)) {}

    std::string  toString() const override { return operation_->toString(); }
    TokenType    getTokenType() const override { return TokenType::OPERATION; }
    unsigned int getPriority() const { return operation_->getPriority(); }
    unsigned int getArity() const { return operation_->getArity(); }

    ADT execute(const ADT& a) const { return operation_->execute(a); }
    ADT execute(const ADT& a, const ADT& b) const {
        return operation_->execute(a, b);
    }

   private:
    std::shared_ptr<Operation> operation_;
};