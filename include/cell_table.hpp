#pragma once

#include <iostream>
#include <vector>

#include "../libs/tabulate.hpp"
#include "cell.hpp"
#include "cell_factory.hpp"

class CellTable {
   public:
    CellTable() : CellTable(0, 0) {}
    CellTable(size_t rows, size_t cols)
        : cells_(rows, std::vector<std::shared_ptr<Cell>>(cols)) {}
    CellTable(const std::vector<std::vector<std::string>>& cells)
        : cells_(cells.size(),
                 std::vector<std::shared_ptr<Cell>>(cells[0].size())) {
        for (size_t row = 0; row < cells.size(); ++row) {
            for (size_t col = 0; col < cells[0].size(); ++col) {
                setCell(row, col,
                        CellFactory::createCell(cells[row][col], this));
            }
        }
    }

    std::shared_ptr<Cell> at(size_t row, size_t col) const;
    std::shared_ptr<Cell> at(std::pair<size_t, size_t> coords) const {
        return at(coords.first, coords.second);
    }
    std::shared_ptr<Cell> at(std::string cell) const {
        return at(parseCoords(cell));
    }

    void setCell(size_t row, size_t col, std::shared_ptr<Cell> cell) {
        if (row >= getRows() || col >= getCols())
            throw std::runtime_error("Index out of bounds");
        cells_[row][col] = cell;
    }

    size_t getRows() const { return cells_.size(); }
    size_t getCols() const { return cells_.empty() ? 0 : cells_[0].size(); }
    void   growTo(size_t rows, size_t cols);
    friend std::ostream& operator<<(std::ostream& os, const CellTable& table);

   private:
    std::vector<std::vector<std::shared_ptr<Cell>>> cells_;

    static std::pair<size_t, size_t> parseCoords(std::string cell);
};