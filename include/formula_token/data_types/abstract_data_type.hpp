#pragma once

#include <variant>

#include "double.hpp"
#include "integer.hpp"
#include "notype.hpp"
#include "string.hpp"

/// @brief Abstract data type for all data types. An std::variant solution was
/// chosen due to efficiency and ease of new operations addition.
using AbstractDataType = std::variant<Integer, Double, String, NoType>;
using ADT              = AbstractDataType;

/// @bug I would like to encapsulate ADT in a class and make parsePrimitive (now
/// in formula.hpp) a member function, but ProgTest won't let me, it doesn't
/// compile

// class AbstractDataType : public std::variant<Integer, Double, String, NoType>
// {
//     using variant::variant;

//    public:
//     static AbstractDataType parsePrimitive(std::string raw_value);
// };
// using ADT = AbstractDataType;
