#pragma once

#include <string>

#include "cell.hpp"
#include "formula.hpp"
#include "formula_token/data_types/abstract_data_type.hpp"

class FormulaCell : public Cell {
   public:
    FormulaCell(std::string formula) : formula_(formula), value_(evaluate()) {}

    AbstractDataType getValue() const override { return value_; }
    AbstractDataType evaluate() const { formula_.evaluate(table_); }

   private:
    Formula                formula_;
    AbstractDataType       value_;
    std::shared_ptr<Table> table_;
};