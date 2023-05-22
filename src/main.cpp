#include <iostream>

#include "../libs/linenoise.h"
#include "cell_table.hpp"

#define MAX_STRING_LENGTH 256

int main() {
    auto table = std::make_shared<CellTable>(
        std::initializer_list<std::initializer_list<std::string>>{
            {"1", "=\"blya \"*20", "3"},
            {"", "=A2", "=B2+1"},
            {"7", "8", "9"}});
    std::cout << *table << std::endl;
}