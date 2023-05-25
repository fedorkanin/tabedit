#pragma once

#include <variant>

#include "double.hpp"
#include "integer.hpp"
#include "notype.hpp"
#include "string.hpp"

using AbstractDataType = std::variant<Integer, Double, String, NoType>;
using ADT              = AbstractDataType;

// class AbstractDataType : public std::variant<Integer, Double, String, NoType>
// {
//     using variant::variant;

//    public:
//     static AbstractDataType parsePrimitive(std::string raw_value);
// };
// using ADT = AbstractDataType;
