#pragma once

#include "../../libs/json.hpp"
#include "../formula_token/formula_tokens.hpp"
#include "operation.hpp"
#include "operation_factory.hpp"

/// @brief Proxy for Operation, used to avoid creating copies of the same
/// operation. TokenVec used in Formula is a vector of unique_ptr, so it would
/// be impossible to use shared_ptr to Operation
class OperationProxy : public FormulaToken {
   public:
    OperationProxy(OperationFactory::OperationName name)
        : operation_(OperationFactory::getOperation(name)) {}

    TokenType    getTokenType() const override { return TokenType::OPERATION; }
    unsigned int getPriority() const { return operation_->getPriority(); }
    unsigned int getArity() const { return operation_->getArity(); }

    /// @brief Execute operation on one argument. Throws exception if arity is
    /// not 1
    ADT          execute(const ADT& a) const { return operation_->execute(a); }
    /// @brief Execute operation on two arguments. Throws exception if arity is
    /// not 2
    ADT          execute(const ADT& a, const ADT& b) const;
    /// @brief Execute operation on vector of arguments. Throws exception if
    /// arity is not equal to vector size
    ADT          execute(const std::vector<ADT>& args) const;

    std::string  toString() const override { return operation_->toString(); }

   private:
    std::shared_ptr<Operation> operation_;
};