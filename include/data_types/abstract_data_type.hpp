#pragma once

#include <memory>

#include "../formula_token.hpp"

class Integer;
class Double;

class AbstractDataType : public FormulaToken {
   public:
    virtual ~AbstractDataType() = default;

    virtual std::unique_ptr<AbstractDataType> clone() const = 0;
    virtual std::unique_ptr<AbstractDataType> getSum(
        const AbstractDataType& data) const = 0;

    virtual std::unique_ptr<AbstractDataType> getSumConcrete(
        const Integer& data) const = 0;
    virtual std::unique_ptr<AbstractDataType> getSumConcrete(
        const Double& data) const = 0;
};
