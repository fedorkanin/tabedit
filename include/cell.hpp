#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "formula.hpp"
#include "formula_token/data_types/abstract_data_type.hpp"
#include "operation/visitors/visitors.hpp"

class Cell {
    using TokenVec = std::vector<std::unique_ptr<FormulaToken>>;

   public:
    Cell(ADT value) : value_(std::move(value)) {}
    Cell(std::string raw_formula)
        : formula_ptr_(std::make_unique<Formula>(raw_formula)) {}
    Cell() = default;

    bool                hasValue() const { return value_.has_value(); }
    bool                hasFormula() const { return formula_ptr_ != nullptr; }
    bool                hasDependants() const { return !dependants_.empty(); }

    void                setValue(ADT value) { value_ = std::move(value); }
    void                setFormula(std::unique_ptr<Formula> formula);
    void                setFormula(std::string raw_formula);
    void                addDependant(CellCoord coord);
    bool                isDependentOn(const CellCoord& coord) const;
    const Formula&      getFormula() const { return *formula_ptr_; }
    std::set<CellCoord> getReferencedCells() const;
    const std::optional<ADT>&  getOptionalValue() const { return value_; }
    const std::set<CellCoord>& getDependants() const { return dependants_; }
    void                       deleteValue() { value_.reset(); }
    void                       deleteFormula() { formula_ptr_.reset(); }
    void        removeDependant(CellCoord coord) { dependants_.erase(coord); }

    /// @brief Return info about cell value, formula and dependants
    std::string dump() const;
    /// @brief Same as dump() but also prints formula RPN
    std::string dumpFull() const;

    std::string toString() const;
    friend void to_json(nlohmann::json& j, const Cell& c);

   private:
    std::optional<ADT>       value_;
    std::unique_ptr<Formula> formula_ptr_;
    std::set<CellCoord>      dependants_;
};
