#include "cell_table.hpp"

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

ADT CellTable::parsePrimitive(std::string raw_value) const {
    // remove leading and trailing whitespace
    raw_value.erase(0, raw_value.find_first_not_of(' '));
    raw_value.erase(raw_value.find_last_not_of(' ') + 1);

    // TODO: error handling in stod may not be preferable
    if (raw_value[0] == '"' && raw_value[raw_value.size() - 1] == '"')
        return String(raw_value.substr(1, raw_value.size() - 2));
    else if (raw_value.find('.') != std::string::npos)
        return Double(std::stod(raw_value));
    else
        return Integer(std::stoi(raw_value));
}

std::shared_ptr<Cell>& CellTable::at(size_t row, size_t col) {
    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds");
    return cells_[row][col];
}

std::shared_ptr<Cell> CellTable::at(size_t row, size_t col) const {
    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds");
    return cells_[row][col];
}

void CellTable::setCell(size_t row, size_t col, std::string value) {
    std::cout << "Setting cell " << row << ", " << col << " to " << value
              << std::endl;

    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds");
    if (cells_[row][col] == nullptr)
        cells_[row][col] = std::make_shared<Cell>();

    auto cell_ptr = cells_[row][col];
    if (value.empty() || value[0] != '=')
        cell_ptr->setValue(parsePrimitive(value));
    else {
        cell_ptr->setFormula(value);
        cell_ptr->setValue(getCellEvaluation(cell_ptr));
    }

    // recalculate dependants
    for (auto& dependant : cell_ptr->getDependants()) {
        std::cout << "Recalculating dependants of " << at(row, col)->toString()
                  << std::endl;
        std::cout << "    " << dependant->toString() << std::endl;
        dependant->setValue(getCellEvaluation(dependant));
        std::cout << "After recalculation:" << std::endl;
        std::cout << "    " << dependant->toString() << std::endl;
    }
}

void CellTable::growTo(size_t rows, size_t cols) {
    if (rows < getRows() || cols < getCols())
        throw std::runtime_error("Cannot shrink table");
    cells_.resize(rows, std::vector<std::shared_ptr<Cell>>(cols));
}

ADT CellTable::getCellEvaluation(std::shared_ptr<Cell> cell) {
    std::cout << "Evaluating cell " << cell->toString() << std::endl;
    // if (cell->hasValue()) return cell->getValue().value();
    // TODO: notype
    if (!cell->hasFormula()) throw std::runtime_error("Evaluating empty cell");

    std::stack<ADT> stack;
    for (auto const& token_ptr : cell->getFormula().getRPN()) {
        OperationProxy* operation = nullptr;
        switch (token_ptr->getTokenType()) {
            case FormulaToken::TokenType::OPERATION: {
                operation = static_cast<OperationProxy*>(token_ptr.get());
                if (operation->getArity() == 1) {
                    if (stack.size() < 1)
                        throw std::runtime_error(
                            "Invalid argument count while "
                            "evaluating: ");
                    ADT operand = std::move(stack.top());
                    stack.pop();
                    stack.push(operation->execute(operand));
                } else {
                    if (stack.size() < 2)
                        throw std::runtime_error(
                            "Invalid argument count while "
                            "evaluating: ");

                    ADT operand2 = std::move(stack.top());
                    stack.pop();
                    ADT operand1 = std::move(stack.top());
                    stack.pop();
                    stack.push(operation->execute(operand1, operand2));
                }
                break;
            }
            case FormulaToken::TokenType::CELL_COORD: {
                std::cout << "Evaluating cell coord" << std::endl;
                std::shared_ptr<Cell>& referenced_cell =
                    at(*static_cast<CellCoord*>(token_ptr.get()));
                // // TODO: handle circular references and non-existing cells
                if (!referenced_cell) {
                    cells_[static_cast<CellCoord*>(token_ptr.get())->getRow()]
                          [static_cast<CellCoord*>(token_ptr.get())->getCol()] =
                              std::make_shared<Cell>();
                }
                auto value_optional = referenced_cell->getValue();

                if (value_optional) {
                    stack.push(std::move(value_optional.value()));
                    // stack.push(String("optional"));
                } else {
                    // TODO: implement notype in ADT
                    stack.push(String("notype"));
                    // throw std::runtime_error(
                    //     "Cell has no value while evaluating: ");
                }
                referenced_cell->addDependant(cell);
                break;
            }
            case FormulaToken::TokenType::INTEGER: {
                stack.push(Integer(
                    static_cast<Integer*>(token_ptr.get())->getValue()));
                break;
            }
            case FormulaToken::TokenType::DOUBLE: {
                stack.push(
                    Double(static_cast<Double*>(token_ptr.get())->getValue()));
                break;
            }
            case FormulaToken::TokenType::STRING: {
                stack.push(
                    String(static_cast<String*>(token_ptr.get())->getValue()));
                break;
            }
            case FormulaToken::TokenType::PARENTHESIS: {
                throw std::runtime_error(
                    "Parenthesis in RPN while evaluating: ");
                break;
            }
        }
    }
    if (stack.size() != 1) {
        throw std::runtime_error("Invalid formula while evaluating: ");
    }
    return stack.top();
}

void CellTable::insertCell(size_t row, size_t col, std::shared_ptr<Cell> cell) {
    if (row >= getRows() || col >= getCols())
        throw std::runtime_error("Index out of bounds");
    cells_[row][col] = cell;
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
