#pragma once

#include <unordered_map>

#include "../formula_token/data_types/abstract_data_type.hpp"
#include "operation.hpp"
#include "visitors/visitors.hpp"

class OperationFactory {
   public:
    enum class OperationName { ADD, SUB, MUL, DIV, SIN, COS, ABS, NEG };
    /// @brief Returns the corresponding operation, NEG and SUB are unary and
    /// binary minus respectively
    static std::shared_ptr<Operation> getOperation(OperationName name);

    /// @brief Returns the corresponding OperationName, throws an exception if
    /// not found
    static OperationName getUnaryOperationName(const std::string& str);
    /// @brief Returns the corresponding OperationName, throws an exception if
    /// not found
    static OperationName getBinaryOperationName(const std::string& str);
    /// @brief Returns the corresponding OperationName, throws an exception if
    /// not found. First searches for unary operation, then for binary operation
    static OperationName getOperationName(const std::string& str);

   private:
    /// @brief Maps OperationName to Operation. The map is used avoid creating
    /// copies of the same operation
    static std::unordered_map<OperationName, std::shared_ptr<Operation>>
        name_to_operation_;

    /// @brief Maps string like "sin" to OperationName
    static std::unordered_map<std::string, OperationName>
        unary_operation_string_to_name_;

    /// @brief Maps string like "+" to OperationName
    static std::unordered_map<std::string, OperationName>
        binary_operation_string_to_name_;
};
