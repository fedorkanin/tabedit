#include <iostream>

#include "data_types/abstract_data_type.hpp"
#include "data_types/double.hpp"
#include "data_types/integer.hpp"
#include "formula.hpp"

int main() {
    Formula formula("1 + 2 + 3");
    std::cout << formula.dumpFull() << std::endl;
    return EXIT_SUCCESS;
}