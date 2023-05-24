#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "../formula_token/data_types/abstract_data_type.hpp"
#include "../formula_token/formula_tokens.hpp"
#include "../../libs/json.hpp"
#include "visitors/visitors.hpp"

class OperationFactory;

class Operation {
    friend class OperationFactory;
    using UnaryFunction  = std::function<ADT(const ADT&)>;
    using BinaryFunction = std::function<ADT(const ADT&, const ADT&)>;

   public:
    unsigned int getPriority() const { return priority_; }
    unsigned int getArity() const { return arity_; }

    ADT         execute(const ADT& a) const;
    ADT         execute(const ADT& a, const ADT& b) const;
    ADT         execute(const std::vector<ADT>& args) const;
    std::string toString() const { return name_; }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Operation, name_);

   private:
    std::variant<UnaryFunction, BinaryFunction> function_;
    std::string                                 name_;
    unsigned int                                priority_;
    unsigned int                                arity_;

    ADT executeUnary(const ADT& a) const;
    ADT executeBinary(const ADT& a, const ADT& b) const;

    Operation(UnaryFunction func, const std::string& name,
              unsigned int priority)
        : function_(func), name_(name), priority_(priority), arity_(1) {}
    Operation(BinaryFunction func, const std::string& name,
              unsigned int priority)
        : function_(func), name_(name), priority_(priority), arity_(2) {}
};