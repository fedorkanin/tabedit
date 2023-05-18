#pragma once

#include <variant>

#include "double.hpp"
#include "integer.hpp"
#include "string.hpp"

// namespace DataTypes {
using AbstractDataType = std::variant<Integer, Double, String>;
using ADT              = AbstractDataType;
// }
