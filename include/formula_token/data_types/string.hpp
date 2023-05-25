#pragma once

#include <memory>
#include <string>

#include "../formula_token.hpp"

class String : public FormulaToken {
   public:
    String(std::string value)
        : value_ptr_(std::make_unique<std::string>(value)) {}
    String(const String& other)
        : value_ptr_(std::make_unique<std::string>(*other.value_ptr_)) {}
    String(String&& other)                       = default;
    String&            operator=(String&& other) = default;

    const std::string& getValue() const { return *value_ptr_; }
    TokenType   getTokenType() const override { return TokenType::STRING; }

    std::string toString() const override { return *value_ptr_; }
    friend void to_json(nlohmann::json& j, const String& p);

   private:
    /// @brief The pointer is used here in order to make ADT smaller as it's
    /// size is the size of the greatest possible member of std::variant used in
    /// ADT
    std::unique_ptr<std::string> value_ptr_;
};