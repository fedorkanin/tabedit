#pragma once

#include "abstract_data_type.hpp"

class AbstractDataType;

/// @brief   CRTP base class for data types
/// @tparam  Derived class, defines the polymorphic methods using CRTP
/// @details This class is used to implement the polymorphic methods of the
///          AbstractDataType class using CRTP. The CRTP pattern is used to
///          avoid the need to implement the polymorphic methods in each
///          derived class.
template <typename Derived>
class DataTypeCRTP : public AbstractDataType {
   public:
    /// @return Unique pointer to a deep copy of the derived class
    std::unique_ptr<AbstractDataType> clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
    /// @brief  Calculate and return the sum of two data types
    /// @return Unique pointer to the data type with sum of two data types
    std::unique_ptr<AbstractDataType> getSum(
        const AbstractDataType& data) const override {
        return data.getSumConcrete(static_cast<const Derived&>(*this));
    }
    std::string toString() const override {
        return std::to_string(static_cast<const Derived&>(*this).value());
    }

   protected:
    /// @brief Default constructor with static type safety check
    DataTypeCRTP() {
        static_assert(std::is_base_of<DataTypeCRTP<Derived>, Derived>::value,
                      "Derived must inherit from DataTypeCRTP<Derived>");
    }
};