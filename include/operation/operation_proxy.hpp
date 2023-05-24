#pragma once

#include "../../libs/json.hpp"
#include "../formula_token/formula_tokens.hpp"
#include "operation.hpp"
#include "operation_factory.hpp"

class OperationProxy : public FormulaToken {
   public:
    OperationProxy(OperationFactory::OperationName name)
        : operation_(OperationFactory::getOperation(name)) {}

    TokenType    getTokenType() const override { return TokenType::OPERATION; }
    unsigned int getPriority() const { return operation_->getPriority(); }
    unsigned int getArity() const { return operation_->getArity(); }

    ADT execute(const ADT& a) const { return operation_->execute(a); }
    ADT execute(const ADT& a, const ADT& b) const {
        return operation_->execute(a, b);
    }
    ADT execute(const std::vector<ADT>& args) const {
        return operation_->execute(args);
    }
    std::string toString() const override { return operation_->toString(); }

    friend void to_json(nlohmann::json& j, const OperationProxy& p) {
        j = nlohmann::json(*p.operation_);
    }

   private:
    std::shared_ptr<Operation> operation_;
};