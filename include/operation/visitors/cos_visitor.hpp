#pragma once

#include <cmath>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct CosVisitor {
    template <typename T>
    AbstractDataType operator()(const T& t) const;
};

template <typename T>
inline AbstractDataType CosVisitor::operator()(const T& t) const {
    using namespace std;
    if constexpr (is_same_v<T, Integer>)
        return Double(cos(t.getValue()));
    else if constexpr (is_same_v<T, Double>)
        return Double(cos(t.getValue()));
    else if constexpr (is_same_v<T, NoType>)
        return NoType();
    else
        throw std::invalid_argument("Invalid operation: cannot cos this type");
}