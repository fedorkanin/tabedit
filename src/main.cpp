#include <iostream>

#include "abstract_data_type.hpp"
#include "double.hpp"
#include "formula.hpp"
#include "integer.hpp"

int main() {
    Formula                formula("1+2 * (4 - 2)");
    std::shared_ptr<Table> table = std::make_shared<Table>();
    std::cout << formula.dumpFull(table) << std::endl;

    std::cout << "Size of operation: " << sizeof(Operation) << std::endl;

    using BinaryFunction = std::function<ADT(const ADT&, const ADT&)>;
    std::cout << "Size of binary function: " << sizeof(BinaryFunction)
              << std::endl;

    std::cout << "Size of operation proxy: " << sizeof(OperationProxy)
              << std::endl;
    // size of shared ptr to operation
    std::cout << "Size of shared ptr to operation: "
              << sizeof(std::unique_ptr<Operation>) << std::endl;

    return EXIT_SUCCESS;
}