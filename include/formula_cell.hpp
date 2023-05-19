#pragma once

#include <string>

#include "cell.hpp"
#include "formula.hpp"
#include "formula_token/data_types/abstract_data_type.hpp"

class CellTable;

class FormulaCell : public Cell {
   public:
    FormulaCell(std::string formula, CellTable* table)
        : formula_(formula), value_(formula_.evaluate(table)), table_(table) {}

    AbstractDataType getValue() const override { return value_; }
    void             evaluateFormula() const { formula_.evaluate(table_); }

   private:
    Formula          formula_;
    AbstractDataType value_;
    CellTable*       table_;
};