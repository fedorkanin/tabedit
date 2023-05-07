#pragma once

#include <memory>
#include <string>

#include "abstract_primitive.hpp"
#include "integer.hpp"
#include "primitive_crtp.hpp"

class OompaLoompa {
   public:
    OompaLoompa(int value) : value_(value) {}

    int value() const { return value_; }

   private:
    int value_;
};

class Double : public PrimitiveCRTP<Double> {
   public:
    Double(float value) : value_(value) {}

    double value() const { return value_; }

   private:
    double value_;

    std::unique_ptr<AbstractPrimitive> getSumConcrete(
        const Integer& data) const override;
    std::unique_ptr<AbstractPrimitive> getSumConcrete(
        const Double& data) const override;
};