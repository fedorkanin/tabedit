#include <iostream>

#include "cell_table.hpp"

int main() {
    auto table = std::make_shared<CellTable>(
        std::initializer_list<std::initializer_list<std::string>>{
            {"=A3", "=A1", "3"}, {"4", "5", "6"}, {"7", "8", "9"}});

    // table->setCell(0, 0, "=2+1+\"hello\" + A2");
    std::cout << *table << std::endl;
    table->setCell(0, 0, "=2+1+\"hello\" +B2");

    std::cout << *table << std::endl;

    return EXIT_SUCCESS;
}