#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "../../libs/json.hpp"
#include "../formula_token/data_types/abstract_data_type.hpp"
#include "../formula_token/formula_tokens.hpp"
#include "visitors/visitors.hpp"

class OperationFactory;

/// @brief Class representing operation in formula. See OperationProxy for
/// methods documentation
class Operation {
    using UnaryFunction  = std::function<ADT(const ADT&)>;
    using BinaryFunction = std::function<ADT(const ADT&, const ADT&)>;

   public:
    unsigned int getPriority() const { return priority_; }
    unsigned int getArity() const { return arity_; }

    ADT          execute(const ADT& a) const;
    ADT          execute(const ADT& a, const ADT& b) const;
    ADT          execute(const std::vector<ADT>& args) const;
    std::string  toString() const { return name_; }

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

    ADT executeUnary(const ADT& a) const;
    ADT executeBinary(const ADT& a, const ADT& b) const;
};