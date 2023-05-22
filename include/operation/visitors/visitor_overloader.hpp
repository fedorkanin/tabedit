#pragma once

/// @class VisitorOverloader
/// @details Visitor overloader.
///
/// This is a utility class that is designed to be used with variants and
/// std::visit to make the syntax of using visitors easier. You can pass in
/// multiple lambdas and it will correctly overload the operator() to make
/// visitation straightforward.
///
/// @tparam Ts The types of the functors that you are overloading.
///
/// @example
///
/// @code{.cpp}
/// std::variant<int, float, std::string> variant = "Hello World!";
///
/// auto visitor = VisitorOverloader{
/// [](int value) { std::cout << "int: " << value << '\n'; },
/// [](float value) { std::cout << "float: " << value << '\n'; },
/// [](const std::string& value) { std::cout << "string: " << value << '\n'; }
/// };
///
/// std::visit(visitor, variant);
/// @endcode
///
/// In this example, we create a std::variant with different types. We then
/// create a visitor that uses our VisitorOverloader class. The
/// VisitorOverloader class takes a set of lambdas which it uses to create an
/// overloaded set of function call operators. When we then use std::visit with
/// our visitor and variant, the correct lambda is called depending on the type
/// that is currently stored in the variant.
template <class... Ts>
struct VisitorOverloader : Ts... {
    using Ts::operator()...;
};

/// @brief Deduction guide for VisitorOverloader
template <class... Ts>
VisitorOverloader(Ts...) -> VisitorOverloader<Ts...>;
