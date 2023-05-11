#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cell_coord.hpp"
#include "data_types/double.hpp"
#include "data_types/integer.hpp"
#include "formula_token.hpp"
#include "operation.hpp"
#include "parenthesis.hpp"

class Formula {
   public:
    Formula(std::string raw_formula) : raw_formula_(std::move(raw_formula)) {
        tokenize();
    }
    void                                       tokenize();
    std::vector<std::unique_ptr<FormulaToken>> getRPN() const;
    std::string                                toString() const;
    std::string                                dumpTokenized() const;

   private:
    std::string                                raw_formula_;
    std::vector<std::unique_ptr<FormulaToken>> tokenized_formula_;
};