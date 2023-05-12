#pragma once

#include "operation.hpp"

class OperationFactory {
   public:
    static std::unique_ptr<Operation> getOperation(std::string name);
    static std::unique_ptr<Operation> getOperation(char name) {
        return getOperation(std::string(1, name));
    }
};