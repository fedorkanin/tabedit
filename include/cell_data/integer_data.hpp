#pragma once

#include <memory>
#include <string>

#include "cell_data.hpp"
#include "data_visitor.hpp"

class IntegerData : public CellData {
   public:
    IntegerData(int value) : value(value) {}

    int  getValue() const { return value; }
    void setValue(int newValue) { value = newValue; }

    std::unique_ptr<CellData> clone() const override {
        return std::make_unique<IntegerData>(value);
    }

    std::unique_ptr<CellData> getSum(const CellData& data) const override {
        return data.getSumConcrete(*this);
    }
    std::string toString() const override { return std::to_string(value); }

   private:
    int value;

    std::unique_ptr<CellData> getSumConcrete(
        const IntegerData& data) const override;
    std::unique_ptr<CellData> getSumConcrete(
        const DoubleData& data) const override;
};