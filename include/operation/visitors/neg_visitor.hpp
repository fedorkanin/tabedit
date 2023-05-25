#pragma once

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct NegationVisitor {
    template <typename T>
    AbstractDataType operator()(const T& t) const;
};

/// @brief Visitor for negation operation (unary minus)
/// @tparam T Operand type
/// @param t Operand
/// @ingroup visitors
/// @return Result of the negation, type depends on the operand
template <typename T>
inline AbstractDataType NegationVisitor::operator()(const T& t) const {
    using namespace std;
    if constexpr (is_same_v<T, Integer>)
        return Integer(-t.getValue());
    else if constexpr (is_same_v<T, Double>)
        return Double(-t.getValue());
    else if constexpr (is_same_v<T, NoType>)
        return NoType();
    else
        throw std::invalid_argument("Invalid operation: cannot neg this type");
}