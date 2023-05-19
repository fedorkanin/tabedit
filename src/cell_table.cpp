#include "cell_table.hpp"

std::shared_ptr<Cell> CellTable::at(size_t row, size_t col) const {
    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds");
    return cells_[row][col];
}

void CellTable::growTo(size_t rows, size_t cols) {
    if (rows < getRows() || cols < getCols())
        throw std::runtime_error("Cannot shrink table");
    cells_.resize(rows, std::vector<std::shared_ptr<Cell>>(cols));
}

std::pair<size_t, size_t> CellTable::parseCoords(std::string cell) {
    size_t row = 0, col = 0;
    for (char c : cell) {
        if (c >= 'A' && c <= 'Z') {
            col *= 26;
            col += c - 'A' + 1;
        } else if (c >= '0' && c <= '9') {
            row *= 10;
            row += c - '0';
        } else {
            throw std::runtime_error("Invalid cell name");
        }
    }
    return {row, col};
}

std::ostream& operator<<(std::ostream& os, const CellTable& table) {
    using namespace tabulate;
    using Row_t =
        std::vector<variant<std::string, const char*, string_view, Table>>;
    Table out_table;
    for (size_t row = 0; row < table.getRows(); ++row) {
        Row_t out_row;
        for (size_t col = 0; col < table.getCols(); ++col) {
            auto cell = table.at(row, col);
            out_row.push_back(cell ? cell->toString() : "");
        }
        out_table.add_row(out_row);
    }
    os << out_table << std::endl;
    return os;
}
