#pragma once

#include <memory>
#include <string>
#include <vector>

#include "formula_token/data_types/abstract_data_type.hpp"
#include "operation/visitors/to_string_visitor.hpp"

class Cell {
   public:
    Cell()                                    = default;
    virtual ~Cell()                           = default;
    virtual AbstractDataType getValue() const = 0;
    virtual std::string      toString() const {
        return std::visit(ToStringVisitor(), getValue());
    }

   protected:
    std::vector<std::shared_ptr<Cell>> dependants_;
};
