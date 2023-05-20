#pragma once

#include <iostream>
#include <vector>

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

    std::shared_ptr<Cell> at(size_t row, size_t col) const;
    std::shared_ptr<Cell> at(CellCoord coord) const {
        return at(coord.getRow(), coord.getCol());
    }

    void   setCell(size_t row, size_t col, std::string value);
    size_t getRows() const { return cells_.size(); }
    size_t getCols() const { return cells_.empty() ? 0 : cells_[0].size(); }
    void   growTo(size_t rows, size_t cols);
    friend std::ostream& operator<<(std::ostream& os, const CellTable& table);

    ADT parsePrimitive(std::string raw_value) const;
    ADT getCellEvaluation(std::shared_ptr<Cell> cell);
    ADT getCellEvaluation(size_t row, size_t col) {
        return getCellEvaluation(at(row, col));
    };

   private:
    // TODO: move from pointers to values for speed
    std::vector<std::vector<std::shared_ptr<Cell>>> cells_;

    void insertCell(size_t row, size_t col, std::shared_ptr<Cell> cell);
};