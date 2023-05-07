#include "data_types/integer.hpp"

std::unique_ptr<AbstractDataType> Integer::getSumConcrete(
    const Integer& data) const {
    return std::make_unique<Integer>(value_ + data.value());
}

std::unique_ptr<AbstractDataType> Integer::getSumConcrete(
    const Double& data) const {
    return std::make_unique<Double>(value_ + data.value());
}
