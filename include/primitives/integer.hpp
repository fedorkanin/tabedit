#pragma once

#include <memory>
#include <string>

#include "primitives/abstract_primitive.hpp"
#include "primitives/double.hpp"
#include "primitives/primitive_crtp.hpp"

class Integer : public PrimitiveCRTP<Integer> {
   public:
    Integer(int value) : value_(value) {}

    int value() const { return value_; }

   private:
    int value_;

    std::unique_ptr<AbstractPrimitive> getSumConcrete(
        const Integer& data) const override;
    std::unique_ptr<AbstractPrimitive> getSumConcrete(
        const Double& data) const override;
};