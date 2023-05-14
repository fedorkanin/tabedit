#pragma once

#include <string>

#include "cell.hpp"

class ValueCell : public Cell {
   public:
    ValueCell(AbstractDataType value) : value_(std::move(value)) {}

    AbstractDataType getValue() const override { return value_; }

   private:
    AbstractDataType value_;
};