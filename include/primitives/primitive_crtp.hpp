#pragma once

#include <iostream>
#include <type_traits>

#include "primitives/abstract_primitive.hpp"

class AbstractPrimitive;

template <typename Derived>
class PrimitiveCRTP : public AbstractPrimitive {
   public:
    std::unique_ptr<AbstractPrimitive> clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
    std::unique_ptr<AbstractPrimitive> getSum(
        const AbstractPrimitive& data) const override {
        return data.getSumConcrete(static_cast<const Derived&>(*this));
    }
    std::string toString() const override {
        return std::to_string(static_cast<const Derived&>(*this).value());
    }

   protected:
    /// @brief Default constructor with static type safety check
    PrimitiveCRTP() {
        static_assert(std::is_base_of<PrimitiveCRTP<Derived>, Derived>::value,
                      "Derived must inherit from PrimitiveCRTP<Derived>");
        std::cout << "Check passed" << std::endl;
    }
};