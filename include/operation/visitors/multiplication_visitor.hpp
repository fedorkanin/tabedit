#pragma once

#include <string>
#include <variant>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct MultiplicationVisitor {
    template <typename T1, typename T2>
    AbstractDataType operator()(const T1& a, const T2& b) const;
};

/// @brief Visitor for multiplication operation
/// @tparam T1 First operand type
/// @tparam T2 Second operand type
/// @param a first operand
/// @param b second operand
/// @return Double or Integer result of the multiplication, String if one of the
/// operands is String and the other is Integer
/// @details String multiplication is python-like, notype multiplication is
/// notype
/// @ingroup visitors
template <typename T1, typename T2>
inline AbstractDataType MultiplicationVisitor::operator()(const T1& a,
                                                          const T2& b) const {
    using namespace std;
    if constexpr (is_same_v<T1, Integer> && is_same_v<T2, Integer>)
        return Integer(a.getValue() * b.getValue());
    else if constexpr ((is_same_v<T1, Integer> && is_same_v<T2, Double>) ||
                       (is_same_v<T1, Double> && is_same_v<T2, Integer>) ||
                       (is_same_v<T1, Double> && is_same_v<T2, Double>)) {
        return Double(a.getValue() * b.getValue());
    }
    // python-like int-string multiplication
    else if constexpr ((is_same_v<T1, Integer> && is_same_v<T2, String>) ||
                       (is_same_v<T1, String> && is_same_v<T2, Integer>)) {
        string result;
        if constexpr (is_same_v<T1, Integer>)
            for (int i = 0; i < a.getValue(); ++i) result += b.getValue();
        else
            for (int i = 0; i < b.getValue(); ++i) result += a.getValue();
        return String(result);
    } else if constexpr (is_same_v<T1, NoType> || is_same_v<T2, NoType>) {
        return NoType();
    } else {
        throw std::invalid_argument(
            "Invalid operation: cannot multiply these types: " + a.toString() +
            " and " + b.toString());
    }
}
