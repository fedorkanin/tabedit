#pragma once

#include <memory>
#include <string>

#include "abstract_data_type.hpp"
#include "data_type_crtp.hpp"
#include "integer.hpp"

class Double : public DataTypeCRTP<Double> {
   public:
    Double(float value) : value_(value) {}
    Double(std::string value) : value_(std::stod(value)) {}

    double value() const { return value_; }

   private:
    double value_;

    std::unique_ptr<AbstractDataType> getSumConcrete(
        const Integer& data) const override;
    std::unique_ptr<AbstractDataType> getSumConcrete(
        const Double& data) const override;
};