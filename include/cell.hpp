#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "formula.hpp"
#include "formula_token/data_types/abstract_data_type.hpp"
#include "operation/visitors/to_string_visitor.hpp"

class Cell {
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    Cell() = default;
    Cell(ADT value) : value_(std::move(value)) {}
    Cell(std::string raw_formula)
        : formula_(std::make_unique<Formula>(raw_formula)) {}

    bool hasValue() const { return value_.has_value(); }
    bool hasFormula() const { return formula_ != nullptr; }

    void setValue(ADT value);
    void setFormula(std::unique_ptr<Formula> formula) {
        formula_ = std::move(formula);
        value_   = std::nullopt;
    }
    void setFormula(std::string raw_formula) {
        setFormula(std::make_unique<Formula>(raw_formula));
    }
    void addDependant(std::shared_ptr<Cell> cell) { dependants_.insert(cell); }
    const std::unordered_set<std::shared_ptr<Cell>>& getDependants() const {
        return dependants_;
    }
    const Formula&            getFormula() const { return *formula_; }
    const std::optional<ADT>& getValue() const { return value_; }

    std::string toString() const;

   private:
    std::optional<ADT>                        value_;
    std::unique_ptr<Formula>                  formula_;
    std::unordered_set<std::shared_ptr<Cell>> dependants_;
};
