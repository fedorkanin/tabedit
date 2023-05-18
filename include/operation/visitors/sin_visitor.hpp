#pragma once

#include <math.h>

#include <string>
#include <variant>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct SinVisitor {
    template <typename T1>
    AbstractDataType operator()(const T1& a) const;
};

template <typename T1>
inline AbstractDataType SinVisitor::operator()(const T1& a) const {
    using namespace std;
    if constexpr (is_same_v<T1, Integer> || is_same_v<T1, Double>)
        return Double(sin(a.getValue()));
    else
        throw std::invalid_argument(
            "Invalid operation: cannot sin these types");
}
