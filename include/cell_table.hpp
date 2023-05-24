#pragma once

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#include "../libs/json.hpp"
#include "../libs/tabulate.hpp"
#include "cell.hpp"
#include "formula_token/cell_coord.hpp"

class CellTable {
   public:
    CellTable() : CellTable(0, 0) {}
    CellTable(size_t rows, size_t cols)
        : cells_(rows, std::vector<std::shared_ptr<Cell>>(cols)) {}
    CellTable(std::initializer_list<std::initializer_list<std::string>> cells);

    std::shared_ptr<Cell>& at(size_t row, size_t col);
    std::shared_ptr<Cell>& at(CellCoord coord) {
        return at(coord.getRow(), coord.getCol());
    }
    const std::shared_ptr<Cell>& at(size_t row, size_t col) const;
    const std::shared_ptr<Cell>& at(CellCoord coord) const {
        return at(coord.getRow(), coord.getCol());
    }

    void setCell(size_t row, size_t col, std::string value);
    void setCell(CellCoord coord, std::string value) {
        setCell(coord.getRow(), coord.getCol(), value);
    }
    size_t getRows() const { return cells_.size(); }
    size_t getCols() const { return cells_.empty() ? 0 : cells_[0].size(); }
    void   growTo(size_t rows, size_t cols);

    friend std::ostream& operator<<(std::ostream& os, const CellTable& table);
    friend void          to_json(nlohmann::json& j, const CellTable& table);
    friend void          from_json(const nlohmann::json& j, CellTable& table);

    ADT  parsePrimitive(std::string raw_value) const;
    void evaluateCell(CellCoord coord, int depth = 0);

   private:
    std::vector<std::vector<std::shared_ptr<Cell>>> cells_;

    void recalcDependants(CellCoord coord, int depth = 0);
    void insertCell(size_t row, size_t col, std::shared_ptr<Cell> cell);
    void evaluateOperationToken(FormulaToken*    token_ptr,
                                std::stack<ADT>& stack);
    void evaluateCellCoordToken(FormulaToken* token_ptr, std::stack<ADT>& stack,
                                CellCoord& coord);
    void evaluateSimpleToken(FormulaToken* token_ptr, std::stack<ADT>& stack);
    std::set<CellCoord> findOutdatedReferences(
        const std::set<CellCoord>& old_references,
        const std::set<CellCoord>& new_references);
    void removeCoordFromDependants(CellCoord                  coord_to_remove,
                                   const std::set<CellCoord>& cells);
};