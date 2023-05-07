#pragma once

#include <memory>

class Integer;
class Double;

class AbstractPrimitive {
   public:
    virtual ~AbstractPrimitive() = default;

    virtual std::unique_ptr<AbstractPrimitive> clone() const = 0;
    virtual std::unique_ptr<AbstractPrimitive> getSum(
        const AbstractPrimitive& data) const = 0;
    virtual std::string toString() const     = 0;

    virtual std::unique_ptr<AbstractPrimitive> getSumConcrete(
        const Integer& data) const = 0;
    virtual std::unique_ptr<AbstractPrimitive> getSumConcrete(
        const Double& data) const = 0;
};
