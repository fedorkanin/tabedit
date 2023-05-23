#pragma once

#include <memory>
#include <string>

#include "../formula_token.hpp"

class String : public FormulaToken {
   public:
    String(std::string value) : value_(std::make_unique<std::string>(value)) {}
    String(const String& other)
        : value_(std::make_unique<std::string>(*other.value_)) {}
    String(String&& other)            = default;
    String& operator=(String&& other) = default;

    /// @todo: const reference?
    std::string getValue() const { return *value_; }
    TokenType   getTokenType() const override { return TokenType::STRING; }
    std::string toString() const override { return *value_; }
    using json = nlohmann::json;
    json toJSON() const override;

   private:
    std::unique_ptr<std::string> value_;
};