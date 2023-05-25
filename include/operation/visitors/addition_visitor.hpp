#pragma once

#include <string>
#include <variant>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct AdditionVisitor {
    template <typename T1, typename T2>
    AbstractDataType operator()(const T1& a, const T2& b) const;
};

/// @brief Visitor for addition operation
/// @tparam T1 First operand type
/// @tparam T2 Second operand type
/// @param a First operand
/// @param b Second operand
/// @ingroup visitors
/// @return Result of the addition, type depends on the operands
/// @details String addition is concatenation, notype addition is notype,
template <typename T1, typename T2>
inline AbstractDataType AdditionVisitor::operator()(const T1& a,
                                                    const T2& b) const {
    using namespace std;

    if constexpr (is_same_v<T1, Integer> && is_same_v<T2, Integer>)
        return Integer(a.getValue() + b.getValue());
    else if constexpr ((is_same_v<T1, Integer> && is_same_v<T2, Double>) ||
                       (is_same_v<T1, Double> && is_same_v<T2, Integer>) ||
                       (is_same_v<T1, Double> && is_same_v<T2, Double>))
        return Double(a.getValue() + b.getValue());
    else if constexpr (is_same_v<T1, NoType> || is_same_v<T2, NoType>)
        return NoType();
    else if constexpr (is_same_v<T1, String> && is_same_v<T2, String>)
        return String(a.getValue() + b.getValue());
    else if constexpr (is_same_v<T1, String>)
        return String(a.getValue() + b.toString());
    else if constexpr (is_same_v<T2, String>)
        return String(a.toString() + b.getValue());

    else
        throw std::invalid_argument(
            "Invalid operation: cannot add these types");
}
