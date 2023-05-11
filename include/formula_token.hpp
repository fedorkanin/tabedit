#pragma once

#include <string>

class FormulaToken {
   public:
    virtual ~FormulaToken()              = default;
    virtual std::string toString() const = 0;
};