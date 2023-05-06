#pragma once

#include <memory>
#include <string>

#include "cell_data.hpp"

class DoubleData : public CellData {
   public:
    DoubleData(float value) : value(value) {}

    float getValue() const { return value; }
    void  setValue(float newValue) { value = newValue; }

    std::unique_ptr<CellData> clone() const override {
        return std::make_unique<DoubleData>(value);
    }

    std::unique_ptr<CellData> getSum(const CellData& data) const override {
        return data.getSumConcrete(*this);
    }
    std::string toString() const override { return std::to_string(value); }

   private:
    float value;

    std::unique_ptr<CellData> getSumConcrete(
        const IntegerData& data) const override;
    std::unique_ptr<CellData> getSumConcrete(
        const DoubleData& data) const override;
};