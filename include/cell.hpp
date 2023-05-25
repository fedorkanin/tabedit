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

    bool           hasValue() const { return value_.has_value(); }
    bool           hasFormula() const { return formula_ptr_ != nullptr; }
    bool           hasDependants() const { return !dependants_.empty(); }

    void           setValue(ADT value) { value_ = std::move(value); }
    void           setFormula(std::unique_ptr<Formula> formula);
    void           setFormula(std::string raw_formula);
    void           addDependant(CellCoord coord);
    bool           isDependentOn(const CellCoord& coord) const;
    const Formula& getFormula() const { return *formula_ptr_; }

    /// @brief Get the cells referenced by the formula, returns empty set if no
    /// formula
    const std::optional<ADT>&  getOptionalValue() const { return value_; }
    const std::set<CellCoord>& getDependants() const { return dependants_; }
    std::set<CellCoord>        getReferencedCells() const;
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
    /// @brief Value is optional because a cell can have dependants but no value
    /// or formula
    std::optional<ADT>       value_;
    /// @brief Formula is stored as a pointer in order to save space when a cell
    /// stores a primitive value
    std::unique_ptr<Formula> formula_ptr_;
    /// @brief Cells that depend on this cell and must be reevaluated when this
    /// cell changes
    std::set<CellCoord>      dependants_;
};
