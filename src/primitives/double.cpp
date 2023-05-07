#include "primitives/double.hpp"

std::unique_ptr<AbstractPrimitive> Double::getSumConcrete(
    const Integer& data) const {
    return std::make_unique<Double>(value_ + data.value());
}

std::unique_ptr<AbstractPrimitive> Double::getSumConcrete(
    const Double& data) const {
    return std::make_unique<Double>(value_ + data.value());
}
