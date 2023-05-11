#pragma once

#include <string>

#include "cell.hpp"

class ValueCell : public Cell {
   public:
    ValueCell(std::unique_ptr<AbstractDataType> value)
        : value_(std::move(value)) {}

    std::unique_ptr<AbstractDataType> getValue() const override {
        return value_->clone();
    }

   private:
    std::unique_ptr<AbstractDataType> value_;
};