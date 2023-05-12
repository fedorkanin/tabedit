#pragma once

#include <functional>
#include <memory>
#include <variant>

#include "../data_types/abstract_data_type.hpp"
#include "../formula_token.hpp"
#include "../visitor_overloader.hpp"

class OperationFactory;

class Operation : public FormulaToken {
    using ADT            = AbstractDataType;
    using ADTptr         = std::unique_ptr<ADT>;
    using UnaryFunction  = std::function<ADTptr(const ADT&)>;
    using BinaryFunction = std::function<ADTptr(const ADT&, const ADT&)>;

   public:
    std::string  toString() const override { return name_; }
    TokenType    getTokenType() const override { return TokenType::OPERATION; }
    unsigned int getPriority() const { return priority_; }

    ADTptr execute(const ADT& a) const;
    ADTptr execute(const ADT& a, const ADT& b) const;

    Operation(UnaryFunction func, const std::string& name,
              unsigned int priority)
        : function_(func), name_(name), priority_(priority), arity_(1) {}
    Operation(BinaryFunction func, const std::string& name,
              unsigned int priority)
        : function_(func), name_(name), priority_(priority), arity_(2) {}

   private:
    std::variant<UnaryFunction, BinaryFunction> function_;
    std::string                                 name_;
    unsigned int                                priority_;
    unsigned int                                arity_;

    ADTptr executeUnary(const ADT& a) const;
    ADTptr executeBinary(const ADT& a, const ADT& b) const;
};