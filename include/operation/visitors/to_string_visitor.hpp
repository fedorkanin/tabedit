#pragma once

#include <stdexcept>
#include <string>
#include <variant>

#include "../../formula_token/data_types/abstract_data_type.hpp"

struct ToStringVisitor {
    template <typename T>
    std::string operator()(const T& variant_instance) const;
};

/// @brief Visitor for toString operation, utilized by the toString method of the operand
/// @tparam T Operand type
/// @param variant_instance Operand
/// @return String result of the toString method of the operand
/// @ingroup visitors
template <typename T>
inline std::string ToStringVisitor::operator()(
    const T& variant_instance) const {
    using namespace std;
    return variant_instance.toString();
}
