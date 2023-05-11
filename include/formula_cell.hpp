#pragma once

#include <string>

#include "cell.hpp"
#include "table.hpp"

class FormulaCell : public Cell {
   public:
    FormulaCell(std::string formula) : formula_(std::move(formula)) {
        evaluate();
    }

    std::unique_ptr<AbstractDataType> getValue() const override {
        return value_->clone();
    }
    void evaluate() const {
        // todo
    }

   private:
    std::string                       formula_;
    std::unique_ptr<AbstractDataType> value_;
    std::shared_ptr<Table>            table_;
};