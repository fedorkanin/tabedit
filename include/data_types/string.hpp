#pragma once

#include "data_type_crtp.hpp"

class String : public DataTypeCRTP<String> {
   public:
    String(std::string value) : value_(value) {}
    std::string value() const { return value_; }

   private:
    std::string value_;
};