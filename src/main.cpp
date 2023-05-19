#include <iostream>

#include "../libs/tabulate.hpp"
#include "abstract_data_type.hpp"
#include "cell_table.hpp"
#include "double.hpp"
#include "formula.hpp"
#include "formula_cell.hpp"
#include "integer.hpp"

int main() {
    auto table = std::make_shared<CellTable>(3, 3);
    table->setCell(0, 0, CellFactory::createCell("=1+2", table.get()));
    std::cout << *table << std::endl;

    return EXIT_SUCCESS;
}