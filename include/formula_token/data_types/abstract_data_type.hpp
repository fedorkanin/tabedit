#pragma once

#include <variant>

#include "double.hpp"
#include "integer.hpp"
#include "notype.hpp"
#include "string.hpp"

using AbstractDataType = std::variant<Integer, Double, String, NoType>;
using ADT              = AbstractDataType;

/// @bug I would like to encapsulate ADT in a class and make parsePrimitive (now
/// in formula.hpp) a member function, but ProgTest won't let me.

// class AbstractDataType : public std::variant<Integer, Double, String, NoType>
// {
//     using variant::variant;

//    public:
//     static AbstractDataType parsePrimitive(std::string raw_value);
// };
// using ADT = AbstractDataType;
