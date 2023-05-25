#pragma once

#include <string>
#include <variant>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct SubtractionVisitor {
    template <typename T1, typename T2>
    AbstractDataType operator()(const T1& a, const T2& b) const;
};

/// @brief Visitor for subtraction operation (binary minus)
/// @tparam T1 First operand type
/// @tparam T2 Second operand type
/// @param a first operand
/// @param b second operand
/// @ingroup visitors
/// @return Double or Integer result of the subtraction
template <typename T1, typename T2>
inline AbstractDataType SubtractionVisitor::operator()(const T1& a,
                                                       const T2& b) const {
    using namespace std;
    if constexpr (is_same_v<T1, Integer> && is_same_v<T2, Integer>)
        return Integer(a.getValue() - b.getValue());
    else if constexpr ((is_same_v<T1, Integer> && is_same_v<T2, Double>) ||
                       (is_same_v<T1, Double> && is_same_v<T2, Integer>) ||
                       (is_same_v<T1, Double> && is_same_v<T2, Double>))
        return Double(a.getValue() - b.getValue());
    else if constexpr (is_same_v<T1, NoType> || is_same_v<T2, NoType>)
        return NoType();
    else
        throw std::invalid_argument(
            "Invalid operation: cannot subtract these types");
}
