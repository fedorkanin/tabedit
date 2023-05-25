#include "cell_table.hpp"

#define MAX_RECURSION_DEPTH 100

CellTable::CellTable(
    std::initializer_list<std::initializer_list<std::string>> cells) {
    growTo(cells.size(), cells.begin()->size());
    size_t row = 0, col = 0;
    for (auto& row_cells : cells) {
        for (auto& cell : row_cells) {
            setCell(row, col, cell);
            ++col;
        }
        ++row;
        col = 0;
    }
}

std::shared_ptr<Cell>& CellTable::at(size_t row, size_t col) {
    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds: " + std::to_string(row) +
                                 " " + std::to_string(col));
    return cells_[row][col];
}

std::shared_ptr<Cell>& CellTable::at(CellCoord coord) {
    return at(coord.getRow(), coord.getCol());
}

const std::shared_ptr<Cell>& CellTable::at(size_t row, size_t col) const {
    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds: " + std::to_string(row) +
                                 " " + std::to_string(col));
    return cells_[row][col];
}

const std::shared_ptr<Cell>& CellTable::at(CellCoord coord) const {
    return at(coord.getRow(), coord.getCol());
}

std::set<CellCoord> CellTable::findOutdatedReferences(
    const std::set<CellCoord>& old_references,
    const std::set<CellCoord>& new_references) {
    std::set<CellCoord> outdated_references;
    std::set_difference(
        old_references.begin(), old_references.end(), new_references.begin(),
        new_references.end(),
        std::inserter(outdated_references, outdated_references.begin()));
    return outdated_references;
}

void CellTable::removeCoordFromDependants(CellCoord coord_to_remove,
                                          const std::set<CellCoord>& cells) {
    for (auto& cell : cells) {
        auto& referenced_cell = at(cell);
        referenced_cell->removeDependant(coord_to_remove);
    }
}

void CellTable::setCell(size_t row, size_t col, std::string value) {
    if (row >= getRows() || col >= getCols()) {
        growTo(std::max(row + 1, getRows()), std::max(col + 1, getCols()));
    }

    if (value.empty()) {
        if (cells_[row][col] == nullptr || !cells_[row][col]->hasDependants()) {
            cells_[row][col] = nullptr;
            return;
        }
        cells_[row][col]->deleteValue();
        cells_[row][col]->deleteFormula();
        return;
    }

    if (cells_[row][col] == nullptr)
        cells_[row][col] = std::make_shared<Cell>();

    auto cell_ptr = cells_[row][col];

    try {
        // try parse as primitive
        cell_ptr->setValue(Formula::parsePrimitive(value));
        recalcDependants(CellCoord(row, col));

        // value changed to primitive, fix old references
        if (!cell_ptr->hasFormula()) return;
        auto old_references = cell_ptr->getFormula().getReferencedCells();
        removeCoordFromDependants(CellCoord(row, col), old_references);

        cell_ptr->deleteFormula();
        return;
    } catch (std::exception&) {
        // parse as formula
    }

    if (!cell_ptr->hasFormula()) {
        cell_ptr->setFormula(value);
        evaluateCell(CellCoord(row, col));
        return;
    }

    auto old_references = cell_ptr->getFormula().getReferencedCells();
    cell_ptr->setFormula(value);
    auto new_references = cell_ptr->getFormula().getReferencedCells();

    // remove old dependants
    std::set<CellCoord> outdated_references =
        findOutdatedReferences(old_references, new_references);
    // remove this cell from the dependants of outdated references
    removeCoordFromDependants(CellCoord(row, col), outdated_references);

    evaluateCell(CellCoord(row, col));
}

void CellTable::setCell(CellCoord coord, std::string value) {
    setCell(coord.getRow(), coord.getCol(), value);
}

void CellTable::growTo(size_t rows, size_t cols) {
    if (rows < getRows() || cols < getCols())
        throw std::runtime_error(
            "Cannot grow to smaller size: " + std::to_string(rows) + " " +
            std::to_string(cols));
    cells_.resize(rows);
    for (auto& row : cells_) row.resize(cols);
}

void CellTable::clear() { cells_.clear(); }

// reduce the size of the table to the minimum required to fit all the cells
void CellTable::shrinkToFit() {
    size_t min_rows = 0, min_cols = 0;
    for (size_t row = 0; row < getRows(); ++row) {
        for (size_t col = 0; col < getCols(); ++col) {
            if (cells_[row][col] == nullptr) continue;
            min_rows = std::max(min_rows, row + 1);
            min_cols = std::max(min_cols, col + 1);
        }
    }
    cells_.resize(min_rows ? min_rows : 1);
    for (auto& row : cells_) row.resize(min_cols ? min_cols : 1);
}

void CellTable::evaluateOperationToken(FormulaToken*    token_ptr,
                                       std::stack<ADT>& stack) {
    OperationProxy* operation = static_cast<OperationProxy*>(token_ptr);
    if (stack.size() < operation->getArity())
        throw std::runtime_error("Invalid argument count while evaluating: ");

    std::vector<ADT> operands;
    operands.reserve(operation->getArity());
    for (int i = operation->getArity() - 1; i >= 0; --i) {
        operands.emplace_back(stack.top());
        stack.pop();
    }
    stack.push(operation->execute(operands));
}

void CellTable::evaluateCellCoordToken(FormulaToken*    token_ptr,
                                       std::stack<ADT>& stack,
                                       CellCoord&       coord) {
    auto referenced_cell_coord = *static_cast<CellCoord*>(token_ptr);
    std::shared_ptr<Cell>& referenced_cell = at(referenced_cell_coord);
    /// @todo: handle circular references and non-existing cells
    if (!referenced_cell) {
        cells_[referenced_cell_coord.getRow()][referenced_cell_coord.getCol()] =
            std::make_shared<Cell>();
    }
    auto value_optional = referenced_cell->getOptionalValue();

    if (value_optional) {
        stack.push(value_optional.value());
    } else {
        stack.push(NoType());
    }

    /// @todo handle dependant addition in setCell
    referenced_cell->addDependant(coord);
}

void CellTable::evaluateSimpleToken(FormulaToken*    token_ptr,
                                    std::stack<ADT>& stack) {
    switch (token_ptr->getTokenType()) {
        case FormulaToken::TokenType::INTEGER: {
            stack.push(Integer(static_cast<Integer*>(token_ptr)->getValue()));
            break;
        }
        case FormulaToken::TokenType::DOUBLE: {
            stack.push(Double(static_cast<Double*>(token_ptr)->getValue()));
            break;
        }
        case FormulaToken::TokenType::STRING: {
            stack.push(String(static_cast<String*>(token_ptr)->getValue()));
            break;
        }
        case FormulaToken::TokenType::NOTYPE: {
            stack.push(NoType());
            break;
        }
        default:
            throw std::runtime_error("Invalid token type: ");
    }
}

void CellTable::evaluateCell(CellCoord coord, int depth) {
    if (depth > MAX_RECURSION_DEPTH) {
        at(coord)->setValue(NoType());
        throw std::runtime_error(
            "Recursion depth exceeded while evaluating cell: " +
            coord.toString());
    }

    auto cell = at(coord);
    if (!cell->hasFormula()) throw std::runtime_error("Evaluating empty cell");

    std::stack<ADT> stack;
    for (auto& token_ptr : cell->getFormula().getRPN()) {
        switch (token_ptr->getTokenType()) {
            case FormulaToken::TokenType::OPERATION: {
                evaluateOperationToken(token_ptr.get(), stack);
                break;
            }
            case FormulaToken::TokenType::CELL_COORD: {
                evaluateCellCoordToken(token_ptr.get(), stack, coord);
                break;
            }
            case FormulaToken::TokenType::PARENTHESIS: {
                throw std::runtime_error(
                    "Parenthesis in RPN while evaluating: ");
                break;
            }
            default: {
                evaluateSimpleToken(token_ptr.get(), stack);
                break;
            }
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Invalid formula while evaluating: " +
                                 cell->getFormula().toString());

    cell->setValue(stack.top());
    // reeval dependants
    recalcDependants(coord, depth + 1);
}

void CellTable::recalcDependants(CellCoord coord, int depth) {
    auto cell = at(coord);
    for (auto& dependant : cell->getDependants())
        evaluateCell(dependant, depth + 1);
}

void CellTable::insertCell(size_t row, size_t col, std::shared_ptr<Cell> cell) {
    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds: " + std::to_string(row) +
                                 ", " + std::to_string(col));
    cells_[row][col] = cell;
}

std::ostream& operator<<(std::ostream& os, const CellTable& table) {
    using namespace tabulate;
    using Row_t =
        std::vector<variant<std::string, const char*, string_view, Table>>;
    Table out_table;

    // create a row of column names like A...Z, AA...AZ, BA...BZ, etc.
    Row_t column_names;
    column_names.reserve(table.getCols() + 1);
    column_names.emplace_back("");
    for (size_t col = 0; col < table.getCols(); ++col)
        column_names.emplace_back(CellCoord::getColName(col));
    out_table.add_row(column_names);

    for (size_t row = 0; row < table.getRows(); ++row) {
        Row_t out_row;
        out_row.reserve(table.getCols() + 1);
        out_row.push_back(std::to_string(row + 1));
        for (size_t col = 0; col < table.getCols(); ++col) {
            auto cell = table.at(row, col);
            out_row.push_back(cell ? cell->toString() : "");
        }
        out_table.add_row(out_row);
    }
    out_table.format().width(10);
    out_table.format().height(1);
    out_table.column(0).format().font_align(FontAlign::center);
    out_table.column(0).format().width(5);
    out_table.row(0).format().font_align(FontAlign::center);
    out_table.row(0).format().padding_left(0);
    // out_table[1][1].format().width(20);
    os << out_table << std::endl;
    return os;
}

void to_json(nlohmann::json& table_json, const CellTable& table) {
    table_json          = nlohmann::json();
    table_json["rows"]  = table.getRows();
    table_json["cols"]  = table.getCols();
    table_json["cells"] = nlohmann::json::array();
    for (size_t row = 0; row < table.getRows(); ++row) {
        for (size_t col = 0; col < table.getCols(); ++col) {
            auto cell = table.at(row, col);
            if (!cell || !cell->hasValue()) continue;

            nlohmann::json cell_json = nlohmann::json::object();
            cell_json[CellCoord(row, col).toString()] = *cell;
            table_json["cells"].push_back(cell_json);
        }
    }
}

void from_json(const nlohmann::json& j, CellTable& table) {
    table = CellTable(j["rows"].get<size_t>(), j["cols"].get<size_t>());
    for (auto& cell_json : j["cells"]) {
        auto cell_obj     = cell_json;
        auto cell_coord   = CellCoord(cell_obj.begin().key());
        auto cell_content = cell_obj.begin().value().get<std::string>();
        table.setCell(cell_coord, cell_content);
    }
}
