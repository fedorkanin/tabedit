#include <iostream>

#include "abstract_data_type.hpp"
#include "double.hpp"
#include "formula.hpp"
#include "integer.hpp"

int main() {
    Formula                formula("1 + 2 * (4 - 2)");
    std::shared_ptr<Table> table = std::make_shared<Table>();
    std::cout << formula.dumpFull(table) << std::endl;
    return EXIT_SUCCESS;
}