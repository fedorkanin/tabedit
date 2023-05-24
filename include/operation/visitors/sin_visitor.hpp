#pragma once

#include <cmath>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct SinVisitor {
    template <typename T>
    AbstractDataType operator()(const T& a) const;
};

/// @brief Visitor for sin operation
/// @tparam T Operand type
/// @param a Operand
/// @return Double result of the sin
/// @ingroup visitors
template <typename T>
inline AbstractDataType SinVisitor::operator()(const T& a) const {
    using namespace std;
    if constexpr (is_same_v<T, Integer> || is_same_v<T, Double>)
        return Double(sin(a.getValue()));
    else if constexpr (is_same_v<T, NoType>)
        return NoType();
    else
        throw std::invalid_argument(
            "Invalid operation: cannot sin these types");
}
