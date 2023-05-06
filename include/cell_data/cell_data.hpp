#pragma once

#include <memory>

#include "double_data.hpp"
#include "integer_data.hpp"

class CellData {
   public:
    virtual ~CellData()                             = default;
    virtual std::unique_ptr<CellData> clone() const = 0;
    virtual std::unique_ptr<CellData> getSum(const CellData& data) const = 0;
    virtual std::string               toString() const                   = 0;

    virtual std::unique_ptr<CellData> getSumConcrete(
        const IntegerData& data) const = 0;
    virtual std::unique_ptr<CellData> getSumConcrete(
        const DoubleData& data) const = 0;
};
