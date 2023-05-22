#pragma once

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "formula.hpp"
#include "formula_token/data_types/abstract_data_type.hpp"
#include "operation/visitors/to_string_visitor.hpp"

class Cell {
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    Cell(ADT value) : value_(std::move(value)) {}
    Cell(std::string raw_formula)
        : formula_(std::make_unique<Formula>(raw_formula)) {}
    Cell() = default;

    bool hasValue() const { return value_.has_value(); }
    bool hasFormula() const { return formula_ != nullptr; }

    void setValue(ADT value) { value_ = std::move(value); }
    void setFormula(std::unique_ptr<Formula> formula) {
        formula_ = std::move(formula);
    }
    void setFormula(std::string raw_formula) {
        setFormula(std::make_unique<Formula>(raw_formula));
    }
    void deleteValue() { value_.reset(); }
    void deleteFormula() { formula_.reset(); }
    void addDependant(CellCoord coord) {
        dependants_.emplace(std::move(coord));
    }
    const std::set<CellCoord>& getDependants() const { return dependants_; }
    void removeDependant(CellCoord coord) { dependants_.erase(coord); }
    const Formula&            getFormula() const { return *formula_; }
    const std::optional<ADT>& getValue() const { return value_; }
    bool                      isDependentOn(const CellCoord& coord) const {
        return formula_ ? formula_->isDependentOn(coord) : false;
    }

    std::string toString() const;

   private:
    std::optional<ADT>       value_;
    std::unique_ptr<Formula> formula_;
    // std::unordered_set<std::shared_ptr<Cell>> dependants_;
    std::set<CellCoord> dependants_;
};
