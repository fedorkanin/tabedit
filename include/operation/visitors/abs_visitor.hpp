#pragma once

#include <cmath>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct AbsVisitor {
    template <typename T>
    AbstractDataType operator()(const T& a) const;
};

template <typename T>
inline AbstractDataType AbsVisitor::operator()(const T& a) const {
    using namespace std;
    if constexpr (is_same_v<T, Integer>)
        return Integer(abs(a.getValue()));
    else if constexpr (is_same_v<T, Double>)
        return Double(abs(a.getValue()));
    else if constexpr (is_same_v<T, NoType>)
        return NoType();
    else
        throw std::invalid_argument(
            "Invalid operation: cannot abs these types");
}
