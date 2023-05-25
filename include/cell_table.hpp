#pragma once

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#include "../libs/json.hpp"
#include "../libs/tabulate.hpp"
#include "cell.hpp"
#include "formula_token/cell_coord.hpp"

/// @brief Class representing a table of cells.
class CellTable {
   public:
    CellTable() : CellTable(0, 0) {}
    CellTable(size_t rows, size_t cols)
        : cells_(rows, std::vector<std::shared_ptr<Cell>>(cols)) {}
    /// @brief Construct a table from a list of lists of strings.
    /// @param cells 2d list of strings representing values or formulas.
    CellTable(std::initializer_list<std::initializer_list<std::string>> cells);

    /// @brief Get a cell at given coordinates with bounds checking.
    std::shared_ptr<Cell>&       at(size_t row, size_t col);
    std::shared_ptr<Cell>&       at(CellCoord coord);
    const std::shared_ptr<Cell>& at(size_t row, size_t col) const;
    const std::shared_ptr<Cell>& at(CellCoord coord) const;

    /// @brief Set the cell at given coordinates to a value or a formula,
    /// with bounds checking. If out of bounds, grows the table.
    /// @param row Row index.
    /// @param col Column index.
    /// @param value String representing a value or a formula.
    /// @details Evaluates a formula if a formula has been set, re-evaluates
    /// dependant cells, manages referenced cells "dependants" member.
    void   setCell(size_t row, size_t col, std::string value);
    void   setCell(CellCoord coord, std::string value);

    size_t getRows() const { return cells_.size(); }
    size_t getCols() const { return cells_.empty() ? 0 : cells_[0].size(); }

    /// @brief Resize the table, keeping the old values.
    /// @param rows New number of rows. If less than the old number, throws an
    /// exception.
    /// @param cols New number of columns. If less than the old number, throws
    /// an exception.
    void   growTo(size_t rows, size_t cols);

    /// @brief Delete all cells.
    void   clear();
    /// @brief Shrink to the minimum size needed to fit all non-empty cells.
    void   shrinkToFit();

    friend std::ostream& operator<<(std::ostream& os, const CellTable& table);
    friend void          to_json(nlohmann::json& j, const CellTable& table);
    friend void          from_json(const nlohmann::json& j, CellTable& table);

    /// @brief Evaluate cell with a formula, reevaluate dependant cells with
    /// recursion depth checking. If no formula, throws an exception.
    void                 evaluateCell(CellCoord coord, int depth = 0);

   private:
    std::vector<std::vector<std::shared_ptr<Cell>>> cells_;

    void recalcDependants(CellCoord coord, int depth = 0);
    void insertCell(size_t row, size_t col, std::shared_ptr<Cell> cell);
    void evaluateOperationToken(FormulaToken*    token_ptr,
                                std::stack<ADT>& stack);
    void evaluateCellCoordToken(FormulaToken* token_ptr, std::stack<ADT>& stack,
                                CellCoord& coord);
    void evaluateSimpleToken(FormulaToken* token_ptr, std::stack<ADT>& stack);
    void removeCoordFromDependants(CellCoord                  coord_to_remove,
                                   const std::set<CellCoord>& cells);
    std::set<CellCoord> findOutdatedReferences(
        const std::set<CellCoord>& old_references,
        const std::set<CellCoord>& new_references);
};