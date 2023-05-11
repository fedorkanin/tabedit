#pragma once

#include <vector>

#include "cell.hpp"

class Table {
   protected:
    std::vector<std::vector<std::shared_ptr<Cell>>> cells_;

   public:
    Table(size_t rows, size_t cols)
        : cells_(rows, std::vector<std::shared_ptr<Cell>>(cols)) {}

    std::shared_ptr<Cell> at(size_t row, size_t col) {
        return cells_[row][col];
    }
};