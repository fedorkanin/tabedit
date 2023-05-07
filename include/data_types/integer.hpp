#pragma once

#include <memory>
#include <string>

#include "abstract_data_type.hpp"
#include "data_type_crtp.hpp"
#include "double.hpp"

class Integer : public DataTypeCRTP<Integer> {
   public:
    Integer(int value) : value_(value) {}

    int value() const { return value_; }

   private:
    int value_;

    std::unique_ptr<AbstractDataType> getSumConcrete(
        const Integer& data) const override;
    std::unique_ptr<AbstractDataType> getSumConcrete(
        const Double& data) const override;
};