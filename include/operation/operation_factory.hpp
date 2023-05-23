#pragma once

#include <unordered_map>

#include "operation.hpp"
#include "visitors/visitors.hpp"

class OperationFactory {
   public:
    static std::shared_ptr<Operation> getOperation(std::string name);
    static std::shared_ptr<Operation> getOperation(char name) {
        return getOperation(std::string(1, name));
    }

    /// @todo Use the map in operation factory!
    static std::unordered_map<std::string, std::shared_ptr<Operation>>
        operations_;
};
