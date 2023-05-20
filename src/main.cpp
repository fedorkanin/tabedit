#include <iostream>

#include "cell_table.hpp"

int main() {
    auto table = std::make_shared<CellTable>(
        std::initializer_list<std::initializer_list<std::string>>{
            {"=2*3+B1", "2", "3"}, {"4", "5", "6"}, {"7", "8", "9"}});

    // table->setCell(0, 0, "=2+1+\"hello\" + A2");
    std::cout << *table << std::endl;

    // print size of Formula, AbstractDataType, FormulaCell, PrimitiveCell
    std::cout << "Size of Formula: " << sizeof(Formula) << std::endl;
    std::cout << "Size of AbstractDataType: " << sizeof(AbstractDataType)
              << std::endl;
    // size of string and vector
    std::cout << "Size of string: " << sizeof(std::string) << std::endl;
    std::cout << "Size of vector: " << sizeof(std::vector<std::string>)
              << std::endl;

    // size of string with content
    std::cout << "Size of string with content: " << sizeof(std::string("hello"))
              << std::endl;

    return EXIT_SUCCESS;
}