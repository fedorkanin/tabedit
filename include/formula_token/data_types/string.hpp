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
    String(String&& other)            = default;
    String& operator=(String&& other) = default;

    /// @todo: const reference?
    std::string getValue() const { return *value_ptr_; }
    TokenType   getTokenType() const override { return TokenType::STRING; }
    std::string toString() const override { return *value_ptr_; }

    friend void to_json(nlohmann::json& j, const String& p) {
        j = nlohmann::json(*p.value_ptr_);
    }

   private:
    std::unique_ptr<std::string> value_ptr_;
};