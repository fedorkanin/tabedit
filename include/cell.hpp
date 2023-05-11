#pragma once

#include <memory>
#include <string>
#include <vector>

#include "data_types/abstract_data_type.hpp"

class Cell {
   protected:
    Cell()                                                     = default;
    virtual ~Cell()                                            = default;
    virtual std::unique_ptr<AbstractDataType> getValue() const = 0;
    virtual std::string toString() const { return getValue()->toString(); }

    std::vector<std::shared_ptr<Cell>> dependants_;
};
