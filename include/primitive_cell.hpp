#pragma once

#include <string>

#include "cell.hpp"

class PrimitiveCell : public Cell {
   public:
    PrimitiveCell(AbstractDataType value) : value_(std::move(value)) {}

    AbstractDataType getValue() const override { return value_; }

   private:
    AbstractDataType value_;
};