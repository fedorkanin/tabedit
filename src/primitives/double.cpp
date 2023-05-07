#include "data_types/double.hpp"

std::unique_ptr<AbstractDataType> Double::getSumConcrete(
    const Integer& data) const {
    return std::make_unique<Double>(value_ + data.value());
}

std::unique_ptr<AbstractDataType> Double::getSumConcrete(
    const Double& data) const {
    return std::make_unique<Double>(value_ + data.value());
}
