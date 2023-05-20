#pragma once

#include <stdexcept>
#include <string>
#include <variant>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct ToStringVisitor {
    template <typename T>
    std::string operator()(const T& variant_instance) const;
};

template <typename T>
inline std::string ToStringVisitor::operator()(
    const T& variant_instance) const {
    using namespace std;
    if constexpr (is_same_v<T, Integer> || is_same_v<T, Double>)
        return std::to_string(variant_instance.getValue());
    else if constexpr (is_same_v<T, String>)
        return variant_instance.getValue();
    else
        throw std::invalid_argument(
            "Invalid operation: cannot convert to string");
}
