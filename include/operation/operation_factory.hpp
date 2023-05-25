#pragma once

#include <unordered_map>

#include "abstract_data_type.hpp"
#include "operation.hpp"
#include "visitors/visitors.hpp"

class OperationFactory {
   public:
    // "-" is a special case, because it can be both unary and binary
    enum class OperationName { ADD, SUB, MUL, DIV, SIN, COS, ABS, NEG };
    static std::shared_ptr<Operation> getOperation(OperationName name);

    static OperationName getUnaryOperationName(const std::string& str);
    static OperationName getBinaryOperationName(const std::string& str);
    static OperationName getOperationName(const std::string& str);

   private:
    /// @todo Use the map in operation factory!
    static std::unordered_map<OperationName, std::shared_ptr<Operation>>
        name_to_operation_;
    static std::unordered_map<std::string, OperationName>
        unary_operation_string_to_name_;
    static std::unordered_map<std::string, OperationName>
        binary_operation_string_to_name_;
};
